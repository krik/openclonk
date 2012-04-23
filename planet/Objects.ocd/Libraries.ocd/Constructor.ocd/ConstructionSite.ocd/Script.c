/**
	ConstructionSite
	Needs material put into it, then constructs the set building.

	@author boni
*/

local definition;
local direction;
local stick_to;
local full_material; // true when all needed material is in the site

public func IsContainer()		{ return !full_material; }
// disallow taking stuff out
public func RefuseTransfer(object toMove) { return true; }

// we have 2 interaction modes
public func IsInteractable(object obj)	{ return definition != nil && !full_material; }
public func GetInteractionCount() { return 2; }
public func GetInteractionMetaInfo(object obj, int num)
{
	if(num == 0)
		return {IconName=nil, IconID=Hammer, Description="$TxtTransfer$"};
	if(num == 1)
		return {IconName=nil, IconID=Icon_Cancel, Description="$TxtAbort$"};
}

public func Construction()
{
	this.visibility = VIS_None;
	definition = nil;
	full_material = false;
	
	return true;
}

public func Set(id def, int dir, object stick)
{
	definition = def;
	direction = dir;
	stick_to = stick;

	var xw = (1-dir*2)*1000;
	
	var w,h;
	w = def->GetDefWidth();
	h = def->GetDefHeight();
	
	SetGraphics(nil, def, 1, GFXOV_MODE_Base);
	SetClrModulation(RGBa(255,255,255,80), 1);
	SetObjDrawTransform(xw,0,0,0,1000, -h*500,1);
	SetGraphics(nil, def, 2, GFXOV_MODE_Base, nil, GFX_BLIT_Wireframe);
	SetObjDrawTransform(xw,0,0,0,1000, -h*500,2);
	SetShape(-w/2, -h, w, h);
	
	SetName(Format("TxtConstruction",def->GetName()));
	
	this.visibility = VIS_Owner | VIS_Allies;
	
	ShowMissingComponents();
}

// only allow collection if needed
public func RejectCollect(id def, object obj)
{
	var max = GetComponent(def, nil, nil, definition);
	
	// not a component?
	if(max == 0)
		return true;
	if(ContentsCount(def) < max)
		return false;
	
	return true;
}

// check if full
public func Collection2(object obj)
{
	// update message
	ShowMissingComponents();
	
	// check if we're done?
	if(full_material)
		StartConstructing();
}

// Interacting removes the Construction site
public func Interact(object clonk, int num)
{
	// Open Contents-Menu
	if(num == 0)
	{
		clonk->CreateContentsMenus();
	}
	// Remove Site
	if(num == 1)
	{
		// test
		for(var obj in FindObjects(Find_Container(this)))
			obj->Exit();
	
		RemoveObject();
	}
}

private func ShowMissingComponents()
{
	if(definition == nil)
	{
		Message("");
		return;
	}
		
	var stuff = GetMissingComponents();
	//var msg = "Construction Needs:";
	var msg = "@";
	for(var s in stuff)
		if(s.count > 0)
			msg = Format("%s %dx{{%i}}", msg, s.count, s.id);
	
	//Message("@%s",msg);
	CustomMessage(msg, this, 0, 0, 23);
}

private func GetMissingComponents()
{
	if(definition == nil)
		return;
	
	if(full_material == true)
		return nil;
	
	// set false again as soon as we find a missing component
	full_material = true;
	
	// check for material
	var comp, index = 0;
	var missing_material = CreateArray();
	while (comp = GetComponent(nil, index, nil, definition))
	{
		// find material
		var max_amount = GetComponent(comp, nil, nil, definition);
		var c = ContentsCount(comp);
		var dif = max_amount-c;
		
		if(dif > 0)
		{
			PushBack(missing_material, {id=comp, count=dif});
			full_material = false;
		}		
		
		index++;
	}
	
	return missing_material;
}

private func StartConstructing()
{
	if(!definition)
		return;
	if(!full_material)
		return;
	
	// create the construction
	var site;
	if(!(site = CreateConstruction(definition, 0, 0, GetOwner(), 1, 1, 1)))
	{
		Interact(nil, 1);
		return;
	}
	
	if(direction)
		site->SetDir(direction);
	// Inform about sticky building
	if (stick_to)
		site->CombineWith(stick_to);
	
	// Autoconstruct 2.0!
	Schedule(site, "DoCon(2)",1,50);
	Schedule(this,"RemoveObject()",1);
}