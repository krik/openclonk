/* Boomshire */

func Initialize()
{

	DrawMaterialQuad("Tunnel",1378,1327-5,1860,1327-5,1860,1330,1387,1330,1);
	for(var i = 1380; i<=1800; i+=28)
		CreateObjectAbove(Dynamite,i,1328);

	Sound("BirdsLoop",true,100,nil,+1);
	Cloud->Place(40);
	
	// Create the parkour goal.
	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Ordered | PARKOUR_CP_Team;
	goal->SetStartpoint(20, 1000);
	goal->AddCheckpoint(760, 950, mode);	
	goal->AddCheckpoint(400, 660, mode);	
	goal->AddCheckpoint(870, 460, mode);	
	goal->AddCheckpoint(1665, 1070, mode);	
	goal->AddCheckpoint(1485, 800, mode);	
	goal->AddCheckpoint(1735, 1410, mode);	
	goal->AddCheckpoint(2110, 1180, mode);
	goal->AddCheckpoint(3350, 1240, mode);	
	goal->AddCheckpoint(3040, 720, mode);
	goal->AddCheckpoint(2530, 520, mode);	
	goal->AddCheckpoint(2150, 510, mode);
	goal->AddCheckpoint(2000, 200, mode);	
	goal->AddCheckpoint(3090, 300, mode);
	var c=goal->AddCheckpoint(1000, 100, mode);
		c->SetCPSize(40);
	var door=CreateObjectAbove(StoneDoor, 135, 60, NO_OWNER);
	door->CloseDoor();
	AddEffect("AutoOpen",door,100,100);
	goal->SetFinishpoint(100, 150, true);
	
	
	//Items
	CreateObjectAbove(Dynamite,1904,878);
	CreateObjectAbove(Dynamite,1905,878);
	CreateObjectAbove(Dynamite,1906,878);
	CreateObjectAbove(Dynamite,2567,918);
	CreateObjectAbove(Dynamite,2568,918);
	CreateObjectAbove(Dynamite,2569,918);
	

	//PlaceEdges();
	Doors();
	Decoration();
	DrawMaterialQuad("Tunnel-brickback",339,687,350,687,350,761,339,761);
	DrawMaterialQuad("Tunnel-brickback",1889,866,1955,866,1955,882,1889,882);
	DrawMaterialQuad("Tunnel-brickback",1920,874,1955,874,1930,1000,1920,1000);
	DrawMaterialQuad("Tunnel-brickback",1920,1000,1930,1000,1970,1100,1960,1100);
	DrawMaterialQuad("Tunnel-brickback",1960,1100,1970,1100,1930,1201,1919,1201);
	
	DrawMaterialQuad("Tunnel-brickback",2549,907,2590,907,2590,920,2549,920);
	DrawMaterialQuad("Tunnel-brickback",2549,920,2560,920,2560,1000,2550,1000);
	DrawMaterialQuad("Tunnel-brickback",2550,1000,2560,1000,2720,1160,2710,1160);
	DrawMaterialQuad("Tunnel-brickback",2710,1160,2720,1160,2710,1190,2700,1190);
	DrawMaterialQuad("Tunnel-brickback",2680,1185,2750,1185,2750,1195,2680,1195);
	DrawMaterialQuad("Tunnel-brickback",2737,1195,2750,1190,2775,1255,2765,1255);
	
	AddEffect("DynamiteEruption",nil,100,130);
	return 1;
}

protected func InitializePlayer(int plr)
{
	SetPlayerTeam(plr, 1);
	return;
}


global func FxDynamiteEruptionTimer(object nobject, effect, int timer)
{
	var dyn=CreateObjectAbove(Firestone,2460+Random(20),670);
	dyn->SetYDir(-80);
	dyn->SetXDir(RandomX(-1,1));
	dyn->SetRDir(RandomX(-30,30));
}

global func FxAutoOpenTimer(object pTarget, effect, int timer)
{
	if(FindObject(Find_ID(Plane),Find_InRect(0,0,2000,500)))
	{
		pTarget->OpenDoor();
		return -1;
	}
	else return 1;
}

global func FxSparklingAttentionTimer(object pTarget, effect, int timer)
{
	CreateParticle("Flash", 0, 0, PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(8, 15), {Prototype = Particles_Flash(), Size = 10}, 10);
}

protected func Decoration()
{
	PlaceObjects(Rock,300,"Earth");
	PlaceObjects(Loam,200,"Earth");
	PlaceObjects(Nugget,100,"Earth");
	{
	var x=[311, 580, 306, 289, 117, 104, 97, 88, 81, 67, 6, -3, 11, 16, 33, 25, 41, 48, 57, 166, 157, 150, 134, 142, 125, 171, 176, 185, 284, 275, 268, 252, 260, 243, 297, 238, 192, 209, 201, 217, 224, 233, 669, 680, 675, 729, 721, 715, 693, 710, 702, 628, 619, 612, 596, 604, 587, 636, 633, 645, 638, 668, 662, 650, 657, 645, 775, 766, 758, 741, 751, 735, 780, 786, 790, 883, 894, 875, 852, 848, 842, 796, 813, 805, 821, 828, 837, 983, 947, 955, 962, 972, 934, 925, 918, 903, 910, 893, 996, 1013, 1006, 1021, 1055, 1047, 1028, 1037, 1063, 1080, 1071, 1090, 930, 923, 938, 945, 968, 958, 975, 985, 906, 898, 915, 821, 990, 982, 973, 929, 942, 954, 912, 904, 920, 861, 845, 853, 735, 747, 740, 768, 777, 752, 758, 821, 815, 828, 834, 790, 782, 798, 804, 252, 290, 282, 276, 268, 262, 297, 96, 144, 133, 153, 113, 126, 54, 44, 102, 86, 66, 77, 146, 131, 136, 111, 123, 259, 263, 270, 279, 251, 242, 239, 231, 222, 168, 157, 188, 194, 202, 208, 213, 177, 373, 377, 385, 392, 399, 410, 420, 324, 367, 359, 349, 341, 335, 279, 271, 261, 287, 295, 305, 313, 617, 608, 577, 595, 601, 571, 555, 563, 536, 528, 544, 496, 485, 491, 517, 507, 500, 458, 450, 440, 466, 432, 406, 395, 416, 424, 714, 695, 705, 655, 647, 675, 639, 632, 529, 537, 546, 584, 576, 557, 566, 592, 825, 834, 839, 864, 857, 847, 849, 878, 889, 872, 906, 937, 884, 865, 874, 944, 857, 831, 822, 841, 849, 243, 236, 230, 222, 213, 207, 200, 190, 152, 161, 165, 172, 181, 382, 427, 395, 401, 406, 413, 420, 387, 379, 388, 397, 372, 363, 353, 439, 449, 492, 485, 476, 467, 457, 499, 509, 518, 527, 578, 552, 543, 534, 562, 570, 585, 669, 661, 633, 642, 651, 622, 613, 604, 595, 677, 1056, 1025, 1027, 1036, 1047, 1033, 999, 984, 969, 979, 989, 959, 950, 945, 937, 1009, 926, 916, 834, 843, 852, 861, 890, 881, 872, 900, 908, 1631, 1626, 1619, 1613, 1575, 1584, 1593, 1602, 1597, 1591, 1581, 1575, 1570, 1561, 1533, 1542, 1551, 1522, 1513, 1504, 1495, 1640, 1631, 1622, 1613, 1604, 1759, 1768, 1777, 1786, 1752, 1743, 1734, 1725, 1648, 1657, 1666, 1675, 1684, 1877, 1868, 1817, 1826, 1835, 1844, 1853, 3546, 3557, 3583, 3575, 3566, 3597, 3606, 3625, 3614, 3525, 3535, 3448, 3439, 3430, 3514, 3474, 3465, 3456, 3485, 3494, 3503, 3387, 3378, 3369, 3322, 3333, 3342, 3351, 3360, 3302, 3311, 3223, 3214, 3205, 3134, 3143, 3152, 3161, 3170, 3179, 3188, 3197, 3290, 3281, 3272, 3263, 3254, 3245, 3234, 2970, 3124, 3115, 3106, 2981, 2990, 2999, 3008, 3017, 3026, 3035, 3044, 3053, 3062, 3071, 3080, 3089, 3098, 2933, 2924, 2915, 2906, 2897, 2888, 2879, 2870, 2861, 2852, 2843, 2834, 2825, 2816, 2941, 2950, 2959, 2805, 2819, 2810, 2801, 2776, 2767, 2758, 2676, 2685, 2694, 2703, 2712, 2721, 2730, 2739, 2748, 2757, 2766, 2775, 2784, 2793, 3645, 3636, 3420, 3411, 3399, 2664, 2655, 2646, 2637, 2628, 2619, 2610, 2601, 2592, 2583, 2574, 2565, 2556, 2547, 2538, 2529, 2520, 2511, 2502, 2493, 2484, 2475, 2466, 2457, 2448, 2439, 2430, 2421, 2412, 2403, 2394, 2385, 2376, 2367, 2358, 2349, 2340, 2331, 2322, 2313, 2304, 2295, 2286, 2277, 2268, 2259, 2250, 2241, 2232, 2223, 2214, 2205, 2196, 2187, 2178, 2169, 2160, 2151, 2142, 2133, 2124, 2115, 2106, 2097, 2088, 2079, 2070, 2061, 2052, 2043, 2034, 2025, 2016, 2007, 1998, 1989, 1980, 1971, 1962, 1953, 1944, 1935, 1926, 1926, 1917, 1908, 1899, 1887, 1017, 900, 684, 684, 666, 621, 612, 603, 477, 468, 432];
	var y=[1241, 1241, 1236, 1210, 1209, 1161, 1154, 1125, 1093, 1065, 1028, 1026, 1021, 1025, 1024, 1028, 1024, 1026, 1025, 1212, 1210, 1209, 1211, 1209, 1210, 1206, 1210, 1210, 1210, 1210, 1208, 1211, 1209, 1210, 1226, 1206, 1210, 1209, 1211, 1209, 1210, 1211, 1186, 1168, 1178, 1194, 1187, 1179, 1165, 1175, 1168, 1240, 1240, 1239, 1240, 1240, 1240, 1239, 1235, 1220, 1228, 1193, 1196, 1210, 1203, 1216, 1225, 1225, 1224, 1216, 1217, 1210, 1227, 1226, 1230, 1218, 1219, 1219, 1222, 1218, 1220, 1219, 1218, 1218, 1217, 1218, 1218, 1186, 1207, 1206, 1205, 1196, 1218, 1217, 1218, 1221, 1219, 1218, 1177, 1178, 1180, 1178, 1177, 1178, 1177, 1178, 1178, 1180, 1178, 1177, 1091, 1085, 1098, 1105, 1117, 1119, 1126, 1126, 1084, 1084, 1089, 803, 915, 918, 955, 996, 988, 974, 997, 996, 995, 1017, 1018, 1018, 1014, 1003, 1008, 977, 982, 995, 991, 996, 988, 1004, 1006, 988, 987, 987, 988, 678, 666, 667, 667, 669, 676, 664, 711, 705, 707, 707, 716, 715, 656, 650, 715, 657, 657, 656, 857, 836, 844, 826, 829, 826, 816, 816, 818, 826, 826, 836, 838, 837, 857, 858, 857, 850, 848, 848, 841, 857, 874, 868, 868, 864, 858, 857, 856, 885, 876, 877, 875, 878, 885, 887, 888, 887, 887, 886, 888, 886, 966, 963, 939, 948, 956, 939, 938, 938, 946, 948, 948, 933, 930, 937, 925, 926, 927, 936, 937, 936, 936, 938, 938, 938, 938, 937, 691, 688, 686, 747, 750, 718, 759, 767, 788, 781, 776, 776, 776, 774, 777, 776, 798, 798, 800, 774, 783, 797, 789, 769, 768, 774, 760, 698, 688, 688, 689, 692, 688, 688, 687, 688, 688, 583, 572, 567, 568, 561, 552, 539, 535, 499, 504, 511, 520, 527, 525, 474, 508, 496, 490, 485, 479, 521, 438, 439, 440, 437, 437, 437, 456, 456, 458, 457, 456, 455, 456, 454, 452, 454, 455, 460, 458, 457, 456, 459, 459, 459, 458, 459, 455, 456, 459, 459, 458, 458, 458, 459, 656, 580, 591, 613, 637, 601, 562, 548, 537, 542, 556, 534, 528, 521, 515, 567, 508, 488, 488, 487, 488, 488, 488, 487, 486, 488, 489, 317, 321, 328, 337, 337, 337, 338, 337, 283, 279, 277, 271, 260, 257, 257, 257, 258, 257, 259, 258, 257, 290, 288, 288, 288, 287, 257, 258, 256, 257, 260, 259, 259, 258, 286, 287, 287, 287, 289, 279, 277, 255, 254, 255, 256, 257, 210, 208, 196, 196, 198, 189, 186, 180, 187, 237, 218, 255, 252, 256, 248, 256, 257, 258, 247, 248, 248, 256, 256, 256, 255, 259, 258, 257, 258, 256, 257, 257, 257, 255, 259, 259, 258, 258, 255, 257, 257, 255, 257, 258, 257, 256, 257, 260, 256, 257, 257, 259, 253, 260, 257, 255, 257, 258, 258, 261, 257, 260, 258, 255, 256, 256, 256, 255, 257, 256, 255, 258, 259, 258, 260, 257, 259, 256, 255, 256, 259, 254, 258, 257, 256, 388, 388, 385, 328, 327, 328, 389, 388, 385, 386, 390, 387, 391, 389, 389, 390, 386, 388, 389, 385, 155, 159, 256, 257, 255, 383, 385, 385, 383, 387, 386, 386, 387, 385, 387, 383, 383, 386, 386, 383, 378, 377, 375, 376, 374, 374, 378, 377, 373, 377, 376, 377, 374, 377, 368, 366, 368, 364, 364, 365, 365, 365, 365, 368, 366, 366, 367, 362, 365, 366, 366, 366, 366, 364, 365, 365, 365, 366, 366, 343, 343, 347, 347, 343, 344, 346, 347, 344, 347, 348, 343, 347, 348, 328, 326, 326, 324, 325, 327, 323, 318, 317, 314, 307, 305, 305, 305, 444, 305, 296, 293, 285, 287, 572, 767, 1163, 705, 735, 767, 765, 765, 926, 930, 463];
	for (var i = 0; i < GetLength(x); i++)
	{
		var gras=CreateObjectAbove(Grass, x[i], y[i] , NO_OWNER);
		while( GBackSolid(x[i],gras->GetY())) gras->SetPosition(x[i],gras->GetY()-1);
		while(!GBackSolid(x[i],gras->GetY())) gras->SetPosition(x[i],gras->GetY()+1);
		gras->SetPosition(x[i],gras->GetY()-2+Random(2));
		gras->SetCategory(C4D_StaticBack);
		gras->DoCon(Random(25));
		if(!Random(20))
		{
			CreateObjectAbove(Fern,gras->GetX(),gras->GetY()+Random(15))->DoCon(-30+Random(100));
			gras->Incinerate();
		}
		
		
	}
	
	}
	
	AddEffect("PlaneReset",CreateObjectAbove(Plane,3030,315,0),100,10,nil,nil);
	AddEffect("PlaneReset",CreateObjectAbove(Plane,3160,315,1),100,10,nil,nil);
}

global func FxPlaneResetTimer(object target, effect, int time)
{
	if(target->GetX() > 3000 || target->Contents())
	{
		effect.count=0;
		return 1;	
	}
	else
		effect.count++;
	
	if(effect.count<4) return 1;
	
	var particles =
	{
		Prototype = Particles_Air(),
		Size = PV_Linear(4, 0)
	};
	DrawParticleLine("Air", target->GetX()+3, target->GetY(), 3030, 315, 1, PV_Random(-2, 2), PV_Random(-2, 2), PV_Random(10, 30), particles);
	
	target->SetPosition(3030,315);
	target->SetR(-90);
	target->SetDir(0);
}

protected func Doors()
{
	var gate = CreateObjectAbove(StoneDoor, 865, 1195, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 1045, 1165, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 1155, 1026, NO_OWNER);
	var	wheel = CreateObjectAbove(ArrowWheel, 1906, 778, NO_OWNER);
	var straw = CreateObjectAbove(DoorTarget,1850,820,NO_OWNER);
	straw->SetGate(gate);
	straw->SetAction("Float");
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 1875, 761, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 1752, 1148, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 1875, 864, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 1116, 1038, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 3115, 685, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 3140, 588, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 585, 915, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 853, 681, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 345, 740, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 60, 644, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 1935, 558, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 1900, 565, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	AddEffect("SparklingAttention",wheel,100,100);
	
	var gate = CreateObjectAbove(StoneDoor, 2965, 316, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 3260, 328, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
	
	var gate = CreateObjectAbove(StoneDoor, 3285, 1150, NO_OWNER);
	var wheel = CreateObjectAbove(SpinWheel, 3220, 1200, NO_OWNER);
	wheel->SetStoneDoor(gate);
	gate->CloseDoor();
		
}

global func PlaceEdges()
{
	var x=[25, 115, 155, 125, 195, 205, 215, 225, 125, 265, 145, 245, 145, 235, 2155, 555, 565, 605, 385, 425, 45, 45, 95, 265, 275, 325, 345, 335, 2535, 2555, 2545, 2545, 2555, 2555, 2545, 2545, 2555, 1795, 1575, 1595, 1695, 1725, 1845, 1855, 1865, 1885, 1925, 1935, 1945, 1955, 1965, 1985, 1995, 1945, 1885, 2415, 2405, 2575, 2565, 2505, 2515, 2665, 2655, 2685, 2695, 2715, 2705, 2735, 2745, 3005, 2955, 2945, 2555, 2855, 2865, 2865, 2875, 2995, 3125, 3015, 3085, 3145, 2785, 2565, 2485, 2515, 2585, 2255, 2235, 2245, 2225, 2195, 2055, 2045, 2035, 2035, 2135, 2085, 1905, 1905, 1935, 1925, 1915, 1925, 1925, 1915, 1935, 1925, 1895, 1915, 1925, 605, 595, 825, 815, 775, 755, 745, 735, 1895, 1915, 1935, 1935, 3385, 3575, 3585, 3595, 3605, 3605, 3595, 3585, 3575, 3555, 3555, 3545, 3525, 3535, 2555, 2565, 2575, 2565, 2585, 2595, 2585, 2575, 2595, 2605, 2615, 2605, 2625, 2635, 2625, 2615, 2635, 2645, 2655, 2645, 2665, 2675, 2665, 2655, 2655, 2675, 2685, 2695, 2685, 2695, 2705, 2745, 2755, 2755, 2765, 2685, 2565, 2565, 2565, 2565, 2555, 2435, 386, 3175, 3085, 3265, 2985, 3265, 3325, 1865, 1985, 1975, 1975, 1985, 2005, 1995, 2055, 2065, 2215, 2175, 2575, 2745, 2735];
	var y=[45, 45, 45, 125, 425, 395, 345, 275, 195, 35, 125, 175, 155, 215, 935, 825, 835, 835, 525, 545, 625, 655, 715, 845, 835, 785, 785, 695, 915, 995, 995, 975, 975, 955, 955, 935, 935, 565, 275, 285, 325, 345, 365, 375, 385, 395, 405, 425, 435, 445, 455, 465, 485, 595, 575, 585, 595, 645, 635, 645, 655, 595, 585, 575, 585, 575, 585, 595, 585, 675, 645, 655, 915, 635, 645, 625, 635, 745, 595, 735, 735, 725, 1235, 1205, 1205, 925, 925, 1045, 1025, 1035, 1015, 975, 845, 865, 895, 1065, 1245, 1245, 1125, 1055, 1025, 1015, 1115, 1105, 1075, 1065, 975, 985, 885, 885, 785, 965, 955, 1005, 995, 985, 995, 1005, 1015, 865, 865, 865, 885, 745, 745, 735, 725, 715, 665, 655, 645, 635, 625, 665, 655, 665, 675, 1015, 1015, 1025, 1025, 1045, 1045, 1035, 1035, 1055, 1055, 1065, 1065, 1085, 1085, 1075, 1075, 1095, 1095, 1105, 1105, 1125, 1125, 1115, 1115, 1105, 1135, 1135, 1145, 1145, 1155, 1155, 1215, 1225, 1215, 1225, 1175, 1225, 1245, 1265, 1295, 1305, 1285, 715, 1085, 585, 275, 275, 335, 395, 275, 535, 545, 885, 865, 755, 775, 715, 705, 995, 965, 935, 945, 945];
	var d=[2, 3, 2, 3, 2, 2, 2, 2, 1, 2, 2, 2, 0, 2, 3, 3, 3, 2, 1, 2, 2, 0, 0, 2, 2, 3, 2, 0, 1, 3, 0, 2, 1, 3, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 3, 2, 1, 1, 2, 2, 1, 1, 2, 0, 3, 3, 0, 0, 0, 3, 2, 3, 3, 0, 0, 0, 0, 0, 1, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 3, 1, 1, 2, 0, 3, 1, 2, 3, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 3, 2, 3, 1, 0, 1, 1, 1, 1, 3, 3, 3, 3, 3, 0, 0, 3, 3, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 3, 0, 3, 3, 0, 0, 3, 0, 0, 3, 3, 3, 1, 1, 1, 1, 1, 0, 0, 3, 1, 3, 2, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 3];
	for (var i = 0; i < GetLength(x); i++)
	{
		var edge=CreateObjectAbove(BrickEdge, x[i], y[i] + 5, NO_OWNER);
		edge->Initialize();
		edge->SetP(d[i]);
		edge->SetPosition(x[i],y[i]);
		edge->PermaEdge();
	}
	return 1;
}

// Gamecall from Race-goal, on respawning.
protected func OnPlayerRespawn(int iPlr, object cp)
{
	var clonk = GetCrew(iPlr);
	clonk->CreateContents(WindBag);
	return;
}

func GetEdges()
{
var x=[];
var y=[];
	for(var e in FindObjects(Find_ID(BrickEdge)))
	{
		x[GetLength(x)]=e->GetX();
		y[GetLength(y)]=e->GetY();
	}
	Log("%v",x);
	Log("%v",y);
}



/* Relaunch */

