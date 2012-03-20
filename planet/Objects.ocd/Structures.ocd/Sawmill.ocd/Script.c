/*--
	Sawmill
	Authors: Ringwaul, Clonkonaut

	Cuts trees or other objects into wood. Accepts only objects purely made from wood.
--*/

#include Library_Ownable
#include Library_Producer

public func Construction()
{
	
	SetProperty("MeshTransformation",Trans_Rotate(-20,0,1,0));
	return _inherited(...);
}

public func Initialize()
{
	this.SpinAnimation = PlayAnimation("work", 10, Anim_Const(0), Anim_Const(1000));
	return _inherited(...);
}

/*-- Interaction --*/

// Sawmill can't be accessed as a container.
public func IsContainer() { return false; }

// Automatically search for trees in front of sawmill
// Temporary solution?
protected func FindTrees()
{
	var tree = FindObject(Find_AtPoint(), Find_Func("IsTree"), Find_Not(Find_Func("IsStanding")));
	if (!tree || GetCon() < 100) return;

	Saw(tree);
}

private func CheckWoodObject(object target)
{
	if (target->GetComponent(nil, 0) != Wood) return false;
	if (target->GetComponent(nil, 1)) return false;
	return true;
}

/*-- Production --*/

private func IgnoreKnowledge() { return true; }

public func Saw(object target)
{
	target->Enter(this);
	var output = target->GetComponent(Wood);
	target->Split2Components();
	AddToQueue(Wood, output);
	return true;
}

private func IsProduct(id product_id)
{
	return product_id->~IsSawmillProduct();
}
private func ProductionTime() { return 100; }
private func PowerNeed() { return 100; }

public func NeedRawMaterial(id rawmat_id)
{
	if (rawmat_id->~IsSawmillProduct())
		return true;
	return false;
}

public func OnProductionStart(id product)
{
	if (!GetEffect("Sawing", this))
	{
		SpinOn();
		AddEffect("Sawing", this, 100, 1, this);
	}
}

public func OnProductionHold(id product)
{
	SpinOff();
	RemoveEffect("Sawing", this);
}

public func OnProductionContinued(id product)
{
	if (!GetEffect("Sawing", this))
	{
		SpinOn();
		AddEffect("Sawing", this, 100, 1, this);
	}
}

public func OnProductionFinish(id product)
{
	if (!GetLength(queue))
	{
		SpinOff();
		RemoveEffect("Sawing", this);
	}
}	

protected func Collection()
{
	Sound("Clonk");
}

public func FxSawingTimer(object target, proplist effect, int time)
{
	if (time >= this.SpinStep * 3 && time % 5)
		CreateParticle("Axe_WoodChip", 6 - Random(3), RandomX(1,4), 5 + Random(11), -RandomX(2,4), 15+Random(10), RGB(255,255,255), this);

	if (!(time % 20))
		Smoke(-10,10,10);
}

public func OnProductEjection(object product)
{
	product->SetPosition(GetX() + 25, GetY() - 8);
	product->SetSpeed(7, 5);
	product->SetR(30 - Random(59));
	Sound("Pop");
}

protected func RejectCollect(id id_def, object collect)
{
	// Don't collect wood
	if (id_def == Wood) 
		return true;
	if (collect->~IsSawmillIngredient() || CheckWoodObject(collect)) 
		return false;
	return true;
}

/*-- Animation --*/

private func SpinOn(int call)
{
	var spin;
	// Slowest spin on first call
	if (!call) { spin = 100; SetMeshMaterial("Beltspin", 1); ClearScheduleCall(this, "SpinOff"); }
	if (call == 1) spin = 75;
	if (call == 2) spin = 50;
	if (call == 3) { spin = 30; SetMeshMaterial("SawmillBlade.Spin", 2); }

	SetAnimationPosition(this.SpinAnimation, Anim_Linear(GetAnimationPosition(this.SpinAnimation), GetAnimationLength("work"), 0, spin, ANIM_Loop));

	if (call < 3) ScheduleCall(this, "SpinOn", this.SpinStep, nil, call+1);
	else Sound("SawmillRipcut", nil, nil, nil, +1);
}

private func SpinOff(int call, int animation_no)
{
	var spin;
	if (!call) { spin = 50; SetMeshMaterial("SawmillBlade", 2); Sound("SawmillRipcut", nil, nil, nil, -1); ClearScheduleCall(this, "SpinOn"); }
	if (call == 1) spin = 75;
	if (call == 2) spin = 100;
	if (call == 3) spin = 150;
	if (call == 4)
	{
		SetMeshMaterial("SawmillBelt", 1);
		SetAnimationPosition(this.SpinAnimation, Anim_Const(GetAnimationPosition(this.SpinAnimation)));
		return;
	}

	SetAnimationPosition(this.SpinAnimation, Anim_Linear(GetAnimationPosition(this.SpinAnimation), GetAnimationLength("work"), 0, spin, ANIM_Loop));

	ScheduleCall(this, "SpinOff", this.SpinStep * 2, nil, call+1);
}

func Definition(def) {
	SetProperty("PictureTransformation", Trans_Mul(Trans_Translate(2000,0,7000),Trans_Rotate(-20,1,0,0),Trans_Rotate(30,0,1,0)), def);
}
local Name = "$Name$";
local SpinStep = 30;