/**
	Dialogue
	
	Attach to a non player charachter to provide a message interface.
*/


local dlg_target;
local dlg_name;
local dlg_info;
local dlg_progress;
local dlg_status;

static const DLG_Status_Active = 0;
static const DLG_Status_Stop = 1;
static const DLG_Status_Remove = 2;


/*-- Dialogue creation --*/

// Sets a new dialogue for a npc.
global func SetDialogue(string name)
{
	if (!this)
		return;
	var dialogue = CreateObject(Dialogue);
	dialogue->InitDialogue(name, this);
	
	dialogue->SetObjectLayer(nil);

	return dialogue;
}

// Removes the existing dialogue of an object.
global func RemoveDialogue()
{
	if (!this)
		return;
		
	var dialogue = FindObject(Find_ID(Dialogue), Find_ActionTarget(this));
	if (dialogue)
		dialogue->RemoveObject();	

	return;
}

/*-- Dialogue properties --*/

protected func Initialize()
{
	// Dialogue progress to one.
	dlg_progress = 1;
	
	dlg_status = DLG_Status_Active;

	return;
}

public func InitDialogue(string name, object target)
{
	dlg_target = target;
	dlg_name = name;

	// Attach dialogue object to target.
	SetAction("Dialogue", target);
	
	// Update dialogue to target.
	UpdateDialogue();
	
	return;
}

private func UpdateDialogue()
{
	// Adapt size to target and its direction.
	var wdt = dlg_target->GetID()->GetDefWidth();
	var hgt = dlg_target->GetID()->GetDefHeight();
	var dir = dlg_target->GetDir();
	SetShape(-wdt/2 + 2*(dir-1)*wdt, -hgt/2, 3*wdt, hgt);
	// Transfer target name.
	//SetName(Format("$MsgSpeak$", dlg_target->GetName()));
	return;
}

public func SetDialogueInfo()
{



	return;
}

public func SetDialogueProgress(int progress)
{
	dlg_progress = Max(1, progress);
	return;
}

public func SetDialogueStatus(int status)
{
	dlg_status = status;
	return;
}

/*-- Interaction --*/

// Players can talk to NPC via the interaction bar.
public func IsInteractable() { return true; }

// Adapt appearance in the interaction bar.
public func GetInteractionMetaInfo(object clonk)
{
	if (InDialogue(clonk))
		return { Description = Format("$MsgSpeak$", dlg_target->GetName()) , IconName = nil, IconID = Clonk, Selected = true };

	return { Description = Format("$MsgSpeak$", dlg_target->GetName()) , IconName = nil, IconID = Clonk, Selected = false };
}

// Called on player interaction.
public func Interact(object clonk)
{
	// Currently in a dialogue: abort that dialogue.
	if (InDialogue(clonk))
		clonk->CloseMenu();	
	
	// No conversation context: abort.
	if (!dlg_name)
		return true;
		
	// Stop dialogue?
	if (dlg_status == DLG_Status_Stop)
	{
		clonk->CloseMenu();
		dlg_status = DLG_Status_Active;
		return true;
	}
	// Remove dialogue?
	if (dlg_status == DLG_Status_Remove)
	{
		clonk->CloseMenu();
		RemoveObject();
		return true;		
	}

	// Start conversation context.
	// Update dialogue progress first.
	var progress = dlg_progress;
	dlg_progress++;
	// Then call relevant functions.
	Call(Format("Dlg_%s_%d", dlg_name, progress), clonk);

	

	return true;
}

private func InDialogue(object clonk)
{
	return clonk->GetMenu() == Dialogue;	
}

private func MessageBox(string message, object clonk, object talker)
{
	// Use current NPC as talker if unspecified.
	if (!talker)
		talker = dlg_target;	
	
	// Use a menu for this dialogue.
	clonk->CreateMenu(Dialogue, this, C4MN_Extra_None, nil, nil, C4MN_Style_Dialog, false, Dialogue);
	
	// Add NPC portrait.
	var portrait = Format("Portrait:%i::%x::%s", Dialogue, talker->GetColor(), "1");
	clonk->AddMenuItem(portrait, "", Dialogue, nil, nil, nil, C4MN_Add_ImgTextSpec);
	
	// Add NPC message.
	var msg = Format("<c %x>%s:</c> %s", talker->GetColor(), talker->GetName(), message);
	clonk->AddMenuItem(msg, "", nil, nil, nil, nil, C4MN_Add_ForceNoDesc);
	
	// Add answers.
	//for (var i = 0; i < GetLength(message.Answers); i++)
	//{
	//	var ans = message.Answers[i][0];
	//	var call_back = message.Answers[i][1];
	//	target->AddMenuItem(ans, call_back, nil, nil, target, nil, C4MN_Add_ForceNoDesc);
	//}
	
	// Set menu decoration.
	clonk->SetMenuDecoration(GUI_MenuDeco);
	
	// Set text progress to NPC name.
	var name = dlg_target->GetName();
	var n_length;
	while (GetChar(name, n_length))
		n_length++;
	clonk->SetMenuTextProgress(n_length + 1);

	return;
}

local ActMap = {
	Dialogue = {
		Prototype = Action,
		Name = "Dialogue",
		Procedure = DFA_ATTACH,
		Delay = 0,
		NextAction = "Dialogue",
	}
};
local Name = "$Name$";