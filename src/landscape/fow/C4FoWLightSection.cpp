
#include "C4Include.h"
#include "C4FoWLightSection.h"
#include "C4FoWBeamTriangle.h"
#include "C4FoWBeam.h"
#include "C4FoWLight.h"
#include "C4FoWRegion.h"
#include "C4Landscape.h"

#include "float.h"

C4FoWLightSection::C4FoWLightSection(C4FoWLight *pLight, int r) : pLight(pLight), iRot(r)
{
	// Rotation matrices
	iRot = r % 360;
	switch(iRot % 360)
	{
		default:
			assert(false);
		case 0:
			a = 1; b = 0; c = 0; d = 1;
			ra = 1; rb = 0; rc = 0; rd = 1;
			break;
		case 90:
			a = 0; b = 1; c = -1; d = 0;
			ra = 0; rb = -1; rc = 1; rd = 0;
			break;
		case 180:
			a = -1; b = 0; c = 0; d = -1;
			ra = -1; rb = 0; rc = 0; rd = -1;
			break;
		case 270: 
			a = 0; b = -1; c =1; d = 0;
			ra = 0; rb = 1; rc = -1; rd = 0;
			break;
	}
	// Beam list
	pBeams = new C4FoWBeam(-1, +1, +1, +1);
}

C4FoWLightSection::~C4FoWLightSection()
{
	ClearBeams();
}

inline void C4FoWLightSection::LightBallExtremePoint(float x, float y, float dir, float &lightX, float &lightY) const
{
	float d = sqrt(x * x + y * y);
	float s = Min(float(pLight->getSize()), d / 5.0f);
	lightX = dir * y * s / d;
	lightY = dir * -x * s / d;
}

inline void C4FoWLightSection::LightBallRightMostPoint(float x, float y, float &lightX, float &lightY) const
	{ LightBallExtremePoint(x,y,+1.0f,lightX,lightY); }

inline void C4FoWLightSection::LightBallLeftMostPoint(float x, float y, float &lightX, float &lightY) const
	{ LightBallExtremePoint(x,y,-1.0f,lightX,lightY); }

template <class T> T C4FoWLightSection::transDX(T dx, T dy) const { return T(a) * dx + T(b) * dy; }
template <class T> T C4FoWLightSection::transDY(T dx, T dy) const { return T(c) * dx + T(d) * dy; }
template <class T> T C4FoWLightSection::transX(T x, T y) const { return transDX(x, y) + T(pLight->getX()); }
template <class T> T C4FoWLightSection::transY(T x, T y) const { return transDY(x, y) + T(pLight->getY()); }

template <class T> T C4FoWLightSection::rtransDX(T dx, T dy) const { return T(ra) * dx + T(rb) * dy; }
template <class T> T C4FoWLightSection::rtransDY(T dx, T dy) const { return T(rc) * dx + T(rd) * dy; }
template <class T> T C4FoWLightSection::rtransX(T x, T y) const { return rtransDX(x-T(pLight->getX()),y-T(pLight->getY())); }
template <class T> T C4FoWLightSection::rtransY(T x, T y) const { return rtransDY(x-T(pLight->getX()),y-T(pLight->getY())); }

void C4FoWLightSection::ClearBeams()
{
	while (C4FoWBeam *beam = pBeams)
	{
		pBeams = beam->getNext();
		delete beam;
	}
}

void C4FoWLightSection::Prune(int32_t reach)
{
	if (reach == 0)
	{
		ClearBeams();
		pBeams = new C4FoWBeam(-1, 1, 1, 1);
		return;
	}
	// TODO PeterW: Merge active pBeams that we have pruned to same length
	for (C4FoWBeam *beam = pBeams; beam; beam = beam->getNext())
		beam->Prune(reach);
}

void C4FoWLightSection::Dirty(int32_t reach)
{
	for (C4FoWBeam *beam = pBeams; beam; beam = beam->getNext())
		if (beam->getLeftEndY() >= reach || beam->getRightEndY() >= reach)
			beam->Dirty(Min(beam->getLeftEndY(), beam->getRightEndY()));
}

C4FoWBeam *C4FoWLightSection::FindBeamLeftOf(int32_t x, int32_t y) const
{
	// Trivial
	y = Max(y, 0);
	if (!pBeams || !pBeams->isRight(x, y))
		return NULL;
	// Go through list
	// Note: In case this turns out expensive, one might think about implementing
	// a skip-list. But I highly doubt it.
	C4FoWBeam *beam = pBeams;
	while (beam->getNext() && beam->getNext()->isRight(x, y))
		beam = beam->getNext();
	return beam;
}

void C4FoWLightSection::Update(C4Rect RectIn)
{
	// Transform rectangle into our coordinate system
	C4Rect Rect = rtransRect(RectIn);
	C4Rect Bounds = rtransRect(C4Rect(0,0,GBackWdt,GBackHgt));

#ifdef LIGHT_DEBUG
	if (!::Game.iTick255) {
		LogSilentF("Full beam list:");
		StdStrBuf beamsString;
		for(C4FoWBeam *beam = pBeams; beam; beam = beam->getNext()) {
			beamsString.ApendBeamChar(' ');
			beamsString.ApendBeam(beam->getDesc());
		}
		LogSilent(beamsString.getData());
	}
#endif

#ifdef LIGHT_DEBUG
	LogSilentF("Update %d/%d-%d/%d", Rect.x, Rect.y, Rect.x+Rect.Wdt, Rect.y+Rect.Hgt);
#endif

	// Out of reach?
	if (Rect.y > pLight->getTotalReach())
		return;

	// will be clipped on rendering anyway, don't bother to run the algorithm
	if (Rect.y + Rect.Hgt < 0)
		return;
	
	// Get last beam that's positively *not* affected
	int32_t ly = RectLeftMostY(Rect),
	        lx = RectLeftMostX(Rect),
	        ry = RectRightMostY(Rect),
	        rx = RectRightMostX(Rect);

	C4FoWBeam *startBeam = FindBeamLeftOf(lx, ly);

	// Skip clean beams
	while (C4FoWBeam *nextBeam = startBeam ? startBeam->getNext() : pBeams) {
		if (nextBeam->isDirty()) break;
		startBeam = nextBeam;
	}
	// Find end beam, determine at which position we have to start scanning
	C4FoWBeam *beam = startBeam ? startBeam->getNext() : pBeams;
#ifdef LIGHT_DEBUG
	if (beam)
		LogSilentF("Start beam is %s", beam->getDesc().getData());
#endif
	C4FoWBeam *endBeam = NULL;
	int32_t startY = Rect.GetBottom();
	while (beam && !beam->isLeft(rx, ry)) {
		if (beam->isDirty() && beam->getLeftEndY() <= Rect.y + Rect.Hgt) {
			endBeam = beam;
			startY = Min(startY, beam->getLeftEndY());
		}
		beam = beam->getNext();
	}

	// Can skip scan completely?
	if (!endBeam)
		return;

	// Update right end coordinates
#ifdef LIGHT_DEBUG
	LogSilentF("End beam is %s", endBeam->getDesc().getData());
#endif

	if (endBeam->isRight(rx, ry)) {
		rx = endBeam->getRightEndX() + 1;
		ry = endBeam->getRightEndY();
		// We want endBeam itself to get scanned
		//assert(!endBeam->isLeft(rx, ry));
	}
#ifdef LIGHT_DEBUG
	//LogSilentF("Right limit at %d, start line %d", 1000 * (iRX - iX) / (iRY - iY), iStartY);
#endif

	// Bottom of scan - either bound by rectangle or by light's reach
	int32_t endY = Min(Rect.GetBottom(), pLight->getReach());

	// Scan lines
	int32_t y;
	for(y = Max(0, startY); y < endY; y++) {

		// Scan all pBeams
		C4FoWBeam *lastBeam = startBeam;
		int32_t dirty = 0;
		for(C4FoWBeam *beam = startBeam ? startBeam->getNext() : pBeams; beam; lastBeam = beam, beam = beam->getNext())
		{
			assert(lastBeam ? lastBeam->getNext() == beam : beam == pBeams);

			// Clean (enough)?
			if (!beam->isDirty() || y < beam->getLeftEndY())
				continue;

			// Out left?
			if (beam->isRight(lx, ly))
				continue;
			// Out right?
			if (beam->isLeft(rx, ry))
				break;

			// We have an active beam that we're about to scan
			dirty++;
			beam->Dirty(y+1);

			// Do a scan
			int32_t xl = Max(beam->getLeftX(y), Bounds.x),
			        xr = Min(beam->getRightX(y), Bounds.x+Bounds.Wdt-1);
			for(int32_t x = xl; x <= xr; x++)
			{
				// Fast free?
				if (!Landscape._FastSolidCheck(transX(x,y), transY(x,y)))
				{
					if (a == 1)
						x = rtransX(Landscape.FastSolidCheckNextX(transX(x,y)) - 1, 0);
					continue;
				}

				// Free?
				if (!GBackSolid(transX(x,y), transY(x,y))) continue;

				// Split points
				int32_t x1 = x - 1, x2 = x + 1;
				bool splitLeft = !beam->isLeft(x1, y);
				bool splitRight = !beam->isRight(x2, y);

				// Double merge?
				if (!splitLeft && !splitRight && lastBeam && beam->getNext())
				{
					if(lastBeam->EliminateRight(x,y))
					{
						beam = lastBeam;
						break; // no typo. fSplitRight => x == xr
					}
				}

				// Merge possible?
				if (!splitLeft && splitRight && lastBeam)
					if (lastBeam->MergeRight(x2, y))
					{
						beam->SetLeft(x2, y);
						assert(beam->isDirty());
						continue;
					}
				if (splitLeft && !splitRight && beam->getNext())
					if (beam->getNext()->MergeLeft(x1, y))
					{
						beam->SetRight(x1, y);
						break; // no typo. fSplitRight => x == xr
					}

				// Split out left
				if (splitLeft)
				{
					lastBeam = beam;
					beam = lastBeam->Split(x1, y);
					assert(lastBeam->getNext() == beam);
				}

				// Split out right
				if(splitRight)
				{
					lastBeam = beam;
					beam = lastBeam->Split(x2, y);
					assert(lastBeam->getNext() == beam);

					// Deactivate left/middle beam
					lastBeam->Clean(y);
					assert(beam->isDirty());
				}
				else
				{
					// Deactivate beam
					beam->Clean(y);
					break;
				}
			}
		}

		// No active pBeams left?
		if (!dirty)
			break;
	}

	// At end of light's reach? Mark all pBeams that got scanned all the way to the end as clean.
	// There's no need to scan them anymore.
	if (y >= pLight->getReach()) {
		for (C4FoWBeam *beam = startBeam ? startBeam->getNext() : pBeams; beam; beam = beam->getNext())
			if (beam->isDirty() && beam->getLeftEndY() > pLight->getReach())
				beam->Clean(pLight->getReach());
	}

#ifdef LIGHT_DEBUG
	LogSilentF("Updated beam list:");
	for(C4FoWBeam *beam = startBeam ? startBeam->getNext() : pBeams; beam; beam = beam->getNext()) {
		if (beam->isLeft(iRX, iRY))
			break;
		LogSilent(beam->getDesc().getData());
	}
#endif
}



bool C4FoWLightSection::isConsistent() const {
	return (a * c + b * d == 1) && (ra * rc + rb * rd == 1) &&
		   (a * ra + b * rc == 1) && (a * rb + b * rd == 0) &&
		   (c * ra + d * rc == 0) && (c * rb + d * rd == 1);
}

void C4FoWLightSection::Invalidate(C4Rect r)
{
	// Assume normalized rectangle
	assert(r.Wdt > 0 && r.Hgt > 0);
	
	// Get rectangle corners that bound the possibly affected pBeams
	int32_t ly = RectLeftMostY(r),
	        lx = RectLeftMostX(r),
	        ry = RectRightMostY(r),
	        rx = RectRightMostX(r);
	C4FoWBeam *lastBeam = FindBeamLeftOf(lx, ly);
	C4FoWBeam *beam = lastBeam ? lastBeam->getNext() : pBeams;

	// Scan over beams
	while (beam && !beam->isLeft(rx, ry))
	{
		// Dirty beam?
		if (beam->getLeftEndY() > r.y || beam->getRightEndY() > r.y)
			beam->Dirty(r.y);

		// Merge with last beam?
		if (lastBeam && lastBeam->isDirty() && beam->isDirty())
		{
			lastBeam->MergeDirty();
			beam = lastBeam->getNext();
		}
		else		// Advance otherwise
		{
			lastBeam = beam;
			beam = beam->getNext();
		}
	}

	// Final check for merging dirty pBeams on the right end
	if (lastBeam && beam && lastBeam->isDirty() && beam->isDirty())
		lastBeam->MergeDirty();

}

int32_t C4FoWLightSection::FindBeamsClipped(const C4Rect &rect, C4FoWBeam *&firstBeam, C4FoWBeam *&endBeam) const
{
	if(rect.y + rect.Hgt < 0) return 0;

	int32_t ly = RectLeftMostY(rect),
	        lx = RectLeftMostX(rect),
	        ry = RectRightMostY(rect),
	        rx = RectRightMostX(rect);

	C4FoWBeam *pPrev = FindBeamLeftOf(lx, ly);
	firstBeam = pPrev ? pPrev->getNext() : pBeams;

	// Find end beam - determine the number of pBeams we actually need to draw
	C4FoWBeam *beam = firstBeam;
	int32_t beamCount = 0;
	while (beam && !beam->isLeft(rx, ry))
	{
		beam = beam->getNext();
		beamCount++;
	}
	endBeam = beam;

	return beamCount;
}

std::list<C4FoWBeamTriangle> C4FoWLightSection::CalculateTriangles(C4FoWRegion *region) const
{
	C4FoWBeam *startBeam = NULL, *endBeam = NULL;
	int32_t beamCount = FindBeamsClipped(rtransRect(region->getRegion()), startBeam, endBeam);
	std::list<C4FoWBeamTriangle> result;

	// no beams inside the rectangle? Good, nothing to render 
	if(!beamCount) return result;

	bool isStartClipped = startBeam != pBeams;
	bool isEndClipped = !!endBeam;

	C4FoWBeam *beam = startBeam;
	for (int32_t i = 0; i < beamCount; i++, beam = beam->getNext())
	{
		C4FoWBeamTriangle tri;
		tri.fanLX = beam->getLeftEndXf();
		tri.fanLY = float(beam->getLeftEndY());
		tri.fanRX = beam->getRightEndXf();
		tri.fanRY = float(beam->getRightEndY());
		if(i == 0 && isStartClipped)
			tri.clipLeft = true;
		if(i == beamCount - 1 && isEndClipped)
		{
			tri.clipRight = true;
		}

		if(tri.fanLX != tri.fanRX || tri.fanLY != tri.fanRY)
			result.push_back(tri);
	}

	if(result.empty()) return result;

	// Phase 1: Project lower point so it lies on a line with outer left/right
	// light lines.
	float scanLevel = 0;
	for (int step = 0; step < 100000; step++)
	{
		// Find the beam to project. This makes this whole algorithm O(n�),
		// but I see no other way to make the whole thing robust :/
		float bestLevel = FLT_MAX;
		for (std::list<C4FoWBeamTriangle>::iterator it = result.begin(), nextIt; it != --result.end(); ++it)
		{
			nextIt = it; ++nextIt;
			float level = Min(it->fanRY, nextIt->fanLY);
			if (level <= scanLevel || level >= bestLevel)
				continue;
			bestLevel = level;
		}
		if(bestLevel == FLT_MAX)
			break;
		scanLevel = bestLevel;

		for(std::list<C4FoWBeamTriangle>::iterator it = result.begin(), nextIt; it != --result.end(); ++it)
		{
			nextIt = it; ++nextIt;
			C4FoWBeamTriangle &tri = *it, &nextTri = *nextIt;

			float level = Min(tri.fanRY, nextTri.fanLY);
			if(level != bestLevel)
				continue;

			// Calculate light bounds. We assume a "smaller" light for closer beams
			float lightLX, lightLY, lightRX, lightRY;
			LightBallLeftMostPoint(tri.fanRX, tri.fanRY, lightLX, lightLY);
			LightBallRightMostPoint(nextTri.fanLX, nextTri.fanLY, lightRX, lightRY);

			// Ascending
			float crossX, crossY;
			bool descendCollision = false;
			if (tri.fanRY > nextTri.fanLY)
			{
				// Left beam surface self-shadowing? We test whether the scalar product
				// of the beam's normal and the light vector is positive.
				if (  (tri.fanRY - tri.fanLY) * (nextTri.fanLX - lightRX) >=
					  (tri.fanRX - tri.fanLX) * (nextTri.fanLY - lightRY))
				{
					// Reduce to upper point (Yep, we know that the upper point
					// must be the right one. Try to figure out why!)
					assert(tri.fanRY <= tri.fanLY);
					tri.fanLX = tri.fanRX;
					tri.fanLY = tri.fanRY;
				}

				// Left beam reduced?
				float fanRXp = tri.fanRX;
				float threshold = 1.0;
				if (tri.fanRX == tri.fanLX && tri.fanRY == tri.fanLY)
				{
					// Move point to the right for the purpose of finding the cross
					// point - after all, given that tri.fanRX == tri.fanLX, we
					// only care about whether to eliminate or insert an additional
					// point for the descend collision, so the exact value doesn't
					// really matter - but we don't want find_cross to bail out!
					fanRXp += 1.0;
					threshold = 0.0;
				}

				// Move right point of left beam to the left (the original point is partly shadowed)
				bool eliminate = false;
				float b;
				bool f = find_cross(lightRX, lightRY, nextTri.fanLX, nextTri.fanLY,
									tri.fanLX, tri.fanLY, fanRXp, tri.fanRY,
									&crossX, &crossY, &b);
			
				// The self-shadow-check should have made sure that the two are
				// never parallel.
				assert(f);

				// Cross point to left of surface? Then the surface itself is
				// shadowed, and we don't need to draw it.
				if (b >= threshold)
				{
					eliminate = true;

				// Cross point actually right of surface? This can happen when
				// we eliminated surfaces. It means that the light doesn't reach
				// down far enough between this and the next beam to hit anything.
				// As a result, we insert a new zero-width surface where the light
				// stops.
				}
				else if (b < 0.0)
				{
					// As it doesn't matter from this point on whether we were
					// in an ascend or a descend case, this gets handled top-level.
					// ... still debating with myself whether a "goto" would be
					// cleaner here ;)
					descendCollision = true;
				}
				else
				{
					// Set cross point
					tri.fanRX = crossX;
					tri.fanRY = crossY;
				}

				// This shouldn't change the case we are in (uh, I think)
				assert(tri.fanRY > nextTri.fanLY);

				// Did we eliminate the surface with this step?
				if (eliminate && it != result.begin())
				{
					// Remove it then (increment iterator so it doesn't get invalidated)
					result.erase(it++);

					// With the elimination, we need to re-process the last
					// beam, as it might be more shadowed than we realized.
					// Note that the last point might have been projected already - 
					// but that's okay
					--it;
				}

			// Descending - same, but mirrored. And without comments.
			}
			else if (tri.fanRY < nextTri.fanLY)
			{
				if (  (nextTri.fanRY - nextTri.fanLY) * (tri.fanRX - lightLX) >=
					  (nextTri.fanRX - nextTri.fanLX) * (tri.fanRY - lightLY))
				{
					assert(nextTri.fanLY <= nextTri.fanRY);
					nextTri.fanRX = nextTri.fanLX;
					nextTri.fanRY = nextTri.fanLY;
				}
				float fanRXp = nextTri.fanRX;
				float threshold = 0.0;
				if (nextTri.fanRX == nextTri.fanLX && nextTri.fanRY == nextTri.fanLY)
				{
					fanRXp += 1.0;
					threshold = 1.0;
				}

				bool eliminate = false;
				float b;
				bool f = find_cross(lightLX, lightLY, tri.fanRX, tri.fanRY,
									nextTri.fanLX, nextTri.fanLY, fanRXp, nextTri.fanRY,
									&crossX, &crossY, &b);
				assert(f);
				if (b <= threshold)
				{
					eliminate = true;
				}
				else if (b > 1.0)
				{
					descendCollision = true;
				}
				else
				{
					nextTri.fanLX = crossX;
					nextTri.fanLY = crossY;
				}
				assert(tri.fanRY < nextTri.fanLY);
				if (eliminate && nextIt != --result.end())
				{
					result.erase(it++);
					if(it != result.begin()) --it;
				}
			}

			if (descendCollision)
			{
				// Should never be parallel -- otherwise we wouldn't be here
				// in the first place.
				bool f = find_cross(lightRX, lightRY, tri.fanRX, tri.fanRY,
									lightLX, lightLY, nextTri.fanLX, nextTri.fanLY,
									&crossX, &crossY);
				assert(f);

				// Ensure some minimum distance to existing points - don't
				// bother with too small bumps. This also catches some floating
				// point inacurracies.
				const float descendEta = 0.5;
				if (crossY <= tri.fanRY + descendEta ||	crossY <= nextTri.fanLY + descendEta)
				  continue;

				// This should always follow an elimination, but better check
				assert(beamCount > result.size());

				C4FoWBeamTriangle newTriangle;
				newTriangle.fanLX = crossX;
				newTriangle.fanLY = crossY;
				newTriangle.fanRX = crossX;
				newTriangle.fanRY = crossY;

				result.insert(nextIt, newTriangle);

				// Jump over surface. Note that our right beam might get
				// eliminated later on, causing us to back-track into this
				// zero-length pseudo-surface. This will cause find_cross
				// above to eliminate the pseudo-surface and back-track
				// further to the left, which is exactly how it should work.
				++it;
			}

		} // end for(std::list<C4FoWBeamTriangle>::iterator it = result.begin(), nextIt = it; it != --result.end(); ++it) loop
	} // end for (int step = 0; step < 100000; step++) loop

	// Phase 2: Calculate fade points
	for (std::list<C4FoWBeamTriangle>::iterator it = result.begin(); it != result.end(); ++it)
	{
		C4FoWBeamTriangle &tri = *it;

		// Calculate light bounds. Note that the way light size is calculated
		// and we are using it below, we need to consider an "asymetrical" light.
		float lightLX, lightLY, lightRX, lightRY;
		LightBallLeftMostPoint(tri.fanLX, tri.fanLY, lightLX, lightLY);
		LightBallRightMostPoint(tri.fanRX, tri.fanRY, lightRX, lightRY);
		
		// This is simply the projection of the left point using the left-most
		// light point, as well as the projection of the right point using the
		// right-most light point.

		// For once we actually calculate this using the real distance
		float dx = tri.fanLX - lightLX;
		float dy = tri.fanLY - lightLY;
		float d = float(pLight->getFadeout()) / sqrt(dx*dx + dy*dy);
		tri.fadeLX = tri.fanLX + d * dx;
		tri.fadeLY = tri.fanLY + d * dy;

		dx = tri.fanRX - lightRX;
		dy = tri.fanRY - lightRY;
		d = float(pLight->getFadeout()) / sqrt(dx*dx + dy*dy);
		tri.fadeRX = tri.fanRX + d * dx;
		tri.fadeRY = tri.fanRY + d * dy;

		// Do the fades cross?
		if ((tri.fadeRX - lightRX) / (tri.fadeRY - lightRY)
			< (tri.fadeLX - lightRX) / (tri.fadeLY - lightRY))
		{
			// Average it
			tri.fadeLX = tri.fadeRX = (tri.fadeLX + tri.fadeRX) / 2;
			tri.fadeLY = tri.fadeRY = (tri.fadeLY + tri.fadeRY) / 2;
		}
	}

	transTriangles(result);
	
	return result;
}

void C4FoWLightSection::transTriangles(std::list<C4FoWBeamTriangle> &triangles) const
{
	for (std::list<C4FoWBeamTriangle>::iterator it = triangles.begin(); it != triangles.end(); ++it)
	{
		C4FoWBeamTriangle &tri = *it;
		float x,y;

		x = tri.fanRX, y = tri.fanRY;
		tri.fanRX = transX(x,y);
		tri.fanRY = transY(x,y);

		x = tri.fanLX, y = tri.fanLY;
		tri.fanLX = transX(x,y);
		tri.fanLY = transY(x,y);

		x = tri.fadeRX, y = tri.fadeRY;
		tri.fadeRX = transX(x,y);
		tri.fadeRY = transY(x,y);

		x = tri.fadeLX, y = tri.fadeLY;
		tri.fadeLX = transX(x,y);
		tri.fadeLY = transY(x,y);
	}
}
