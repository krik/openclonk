/* Automatically created objects file */

static g_goal, g_object_fade, g_statue, g_doorleft, g_doorright;

func InitializeObjects()
{
	CreateObject(Rule_BuyAtFlagpole, 0, 0);
	CreateObject(Rule_TeamAccount, 0, 0);

	g_goal = CreateObject(Goal_ProtectTheStatue, 0, 0);
	g_goal.StaticSaveVar = "g_goal";

	g_object_fade = CreateObject(Rule_ObjectFade, 0, 0);
	g_object_fade.StaticSaveVar = "g_object_fade";

	g_statue = CreateObjectAbove(Idol, 632, 445);
	g_statue.StaticSaveVar = "g_statue";

	g_doorleft = CreateObjectAbove(StoneDoor, 495, 449);
	g_doorleft->SetComDir(COMD_Down);
	g_doorleft.StaticSaveVar = "g_doorleft";
	g_doorleft->SetAutoControl();
	g_doorright = CreateObjectAbove(StoneDoor, 765, 448);
	g_doorright->SetComDir(COMD_Down);
	g_doorright.StaticSaveVar = "g_doorright";
	g_doorright->SetAutoControl();

	var Flagpole0012 = CreateObjectAbove(Flagpole, 676, 369);
	Flagpole0012->SetCategory(C4D_StaticBack);
	Flagpole0012->SetColor(0xff);
	var Flagpole0007 = CreateObjectAbove(Flagpole, 582, 369);
	Flagpole0007->SetCategory(C4D_StaticBack);
	Flagpole0007->SetColor(0xff);

	CreateObjectAbove(Rock, 312, 713);
	CreateObjectAbove(Rock, 353, 679);
	CreateObjectAbove(Rock, 894, 707);
	CreateObjectAbove(Rock, 1084, 582);

	CreateObjectAbove(Loam, 752, 608);
	CreateObjectAbove(Loam, 130, 555);

	CreateObjectAbove(Firestone, 436, 652);
	CreateObjectAbove(Firestone, 554, 533);
	CreateObjectAbove(Firestone, 199, 583);
	CreateObjectAbove(Firestone, 883, 559);
	return true;
}
