/**
	Constructor
	Library for objects which are able to construct structures.
	
	@author Maikel
*/


public func IsConstructor() { return true; }


public func ControlUseStart(object clonk, int x, int y)
{
	// Is the clonk able to construct?
	if(clonk->GetProcedure() != "WALK")
	{
		clonk->CancelUse();
		return true;
	}
	// Is the clonk at an construction site?
	var structure = FindObject(Find_Category(C4D_Structure), Find_Or(Find_Distance(20), Find_AtPoint()), Find_Layer(GetObjectLayer()));
	if (structure)
	{
		if (structure->GetCon() < 100)
		{
			Construct(clonk, structure);
			return true;
		}
		if (structure->GetDamage() > 0)
		{
			Repair(clonk, structure);
			return true;
		}
	}
	
	// Otherwise create a menu with possible structures to build.
	clonk->CreateConstructionMenu(this);
	clonk->CancelUse();
	return true;
}

public func HoldingEnabled() { return true; }

public func ControlUseHolding(object clonk, int x, int y)
{
	// Is the clonk still able to construct?
	if (clonk->GetProcedure() != "WALK")
	{
		clonk->CancelUse();
		return true;
	}
	// Is the clonk still at an construction site?
	var structure = FindObject(Find_Category(C4D_Structure), Find_Or(Find_Distance(20), Find_AtPoint()), Find_Layer(GetObjectLayer()));
	if (structure)
	{	
		if (structure->GetCon() < 100)
		{
			Construct(clonk, structure);
			return true;
		}
		if (structure->GetDamage() > 0)
		{
			Repair(clonk, structure);
			return true;
		}
	}	
	clonk->CancelUse();
	return true;
}

private func Construct(object clonk, object structure)
{
	// Look for missing components.
	var structure_id = structure->GetID();
	var con = structure->GetCon();
	var comp, index = 0;
	var can_construct =  true;
	while (comp = structure->GetComponent(nil, index))
	{
		var max_amount = GetComponent(comp, nil, nil, structure_id);
		// Try to transfer components from constructing clonk to the structure.
		for (var i = 0; i < max_amount - structure->GetComponent(comp); i++)
		{
			var content = FindObject(Find_Container(clonk), Find_ID(comp));
			if (content)
			{
				clonk->Message("Used {{%i}}", comp);
				content->RemoveObject();
				structure->SetComponent(comp, structure->GetComponent(comp) + 1);
			}
		}		
		// Check if there now is enough material for current con, if so the construction can continue.
		if (100 * structure->GetComponent(comp) / max_amount < con)
			can_construct = false;
		index++;
	}
	// Continue construction if possible.
	if (can_construct)
	{
		structure->DoCon(1);
		clonk->Message("Constructing %d%", structure->GetCon());
	}
	// Otherwise show missing construction materials.
	else
	{
		ShowConstructionMaterial(clonk, structure);
		clonk->CancelUse();
	}
	return;
}

private func ShowConstructionMaterial(object clonk, object structure)
{
	var mat_msg = "Construction needs ";
	var structure_id = structure->GetID();
	var comp, index = 0;
	while (comp = structure->GetComponent(nil, index))
	{
		var current_amount = structure->GetComponent(comp);
		var max_amount = GetComponent(comp, nil, nil, structure_id);
		mat_msg = Format("%s %dx{{%i}}", mat_msg, Max(0, max_amount - current_amount), comp);
		index++;
	}
	clonk->Message(mat_msg);
	return;
}


private func Repair(object clonk, object structure)
{

}

/** Gives a list of ids of the players knowledge.
*/
public func GetConstructionPlans(int plr)
{
	var construction_plans = [];
	var construct_id, index = 0;
	while (construct_id = GetPlrKnowledge(plr, 0, index++, C4D_Structure))
		construction_plans[index-1] = construct_id;
	return construction_plans;
}

protected func CreateConstructionSite(object clonk, id structure_id)
{
	// Only when the clonk is standing and outdoors
	if (clonk->GetAction() != "Walk")
		return false;
	if (clonk->Contained()) 
		return false;
	// Check if the building can be build here
	if (structure_id->~RejectConstruction(0, 10, clonk)) 
		return false;
	// Set owner for CreateConstruction
	SetOwner(clonk->GetOwner());
	// Create construction site
	var site;
	if (!(site = CreateConstruction(structure_id, 0, 10, Contained()->GetOwner(), 1, 1, 1)))
		return false;
	// Message
	clonk->Message("$TxtConstructions$", site->GetName());
	return true;
}
