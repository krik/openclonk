/*
 * OpenClonk, http://www.openclonk.org
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

#include <C4Include.h>
#include <C4Fullscreen.h>
#include <C4GraphicsSystem.h>
#include <C4Viewport.h>
#include <C4Console.h>
#include <C4Game.h>

#import <C4DrawGL.h>

#import "ClonkMainMenuActions.h"
#import "ClonkOpenGLView.h"
#import "ConsoleWindowController.h"

@implementation ClonkAppDelegate (ClonkMainMenuActions)

- (IBAction) openScenario:(id)sender
{
	Console.FileOpen();
}

- (IBAction) openScenarioWithPlayers:(id)sender
{
	Console.FileOpenWPlrs();
}

- (IBAction) closeScenario:(id)sender
{
	Console.FileClose();
	[consoleController.window setRepresentedFilename:@""];
}


- (IBAction) saveGameAs:(id)sender
{
	Console.FileSaveAs(true);
}

- (IBAction) saveScenario:(id)sender
{
	Console.FileSave();
}

- (IBAction) saveScenarioAs:(id)sender
{
	Console.FileSaveAs(false);
}

- (IBAction) record:(id)sender
{
	Console.FileRecord();
}

- (IBAction) newViewport:(id)sender
{
	Console.ViewportNew();
}

- (IBAction) newViewportForPlayer:(id)sender
{
	::Viewports.CreateViewport([sender tag]);
}

- (IBAction) joinPlayer:(id)sender
{
	Console.PlayerJoin();
}

- (IBAction) openPropTools:(id)sender;
{
	Console.EditCursor.OpenPropTools();
}

- (IBAction) showAbout:(id)sender;
{
	Console.HelpAbout();
}

- (IBAction) toggleFullScreen:(id)sender
{
	if (Application.isEditor)
	{
		NSBeep();
		return;
	}
	[gameWindowController setFullscreen:Config.Graphics.Windowed];
	Config.Graphics.Windowed = !Config.Graphics.Windowed;
	
}

- (IBAction) togglePause:(id)sender
{
	[self simulateKeyPressed:K_PAUSE];
}

- (IBAction) setConsoleMode:(id)sender
{
	[consoleController selectMode:sender];
	[consoleController.modeSelector selectSegmentWithTag:[sender tag]];
}

- (IBAction) setDrawingTool:(id)sender
{
	[consoleController selectTool:sender];
	[consoleController.toolSelector selectSegmentWithTag:[sender tag]];
}

- (IBAction) suggestQuitting:(id)sender;
{
	if (!Application.isEditor && Game.IsRunning)
	{
		NSLog(@"Game running, only simulating Esc key");
		[self simulateKeyPressed:K_ESCAPE];
		return;
	}
	else
	{
		if (Application.isEditor)
			Console.FileClose();
		Application.fQuitMsgReceived = true;
		return;
	}
}

- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)item
{
	// enabled when running in fullscreen
	if ([item action] == @selector(toggleFullScreen:))
		return !Application.isEditor;
	
	// game running no matter whether console or fullscreen
	if ([item action] == @selector(togglePause:))
		return Game.IsRunning;

	// enabled when game running and console mode
	SEL gameRunningInConsoleModeSelectors[] =
	{
		@selector(saveScenario:),
		@selector(saveScenarioAs:),
		@selector(saveGameAs:),
		@selector(record:),
		@selector(closeScenario:),
		@selector(newViewport:),
		@selector(newViewportForPlayer:),
		@selector(joinPlayer:),
		@selector(openPropTools:),
		@selector(setConsoleMode:),
		@selector(setDrawingTool:),
		nil
	};
	int i = 0;
	SEL s;
	while ((s = gameRunningInConsoleModeSelectors[i++]) != nil)
	{
		if (s == [item action])
			return Application.isEditor && Game.IsRunning;
	}
	
	// always enabled
	return YES;
}

- (IBAction) visitWebsite:(id)sender;
{
	OpenURL("http://wiki.openclonk.org");
}

- (void) simulateKeyPressed:(C4KeyCode)key
{
	Game.DoKeyboardInput(key, KEYEV_Down, false, false, false, false, NULL);
	Game.DoKeyboardInput(key, KEYEV_Up,   false, false, false, false, NULL);
}

@end