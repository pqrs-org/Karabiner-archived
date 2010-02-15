// -*- Mode: objc -*-
//
//  KeyDumpAppDelegate.h
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import <Cocoa/Cocoa.h>
#import "EventQueue.h"
#import "OtherInformationStore.h"
#import "server_objc_part.h"

@interface KeyDumpAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet id keyResponder_;
  IBOutlet OtherInformationStore* otherinformationstore_;
  IBOutlet ServerObjcPart* serverobjcpart_;
}

@property (assign) IBOutlet NSWindow* window;
@property (assign) ServerObjcPart* serverobjcpart_;
@property (assign) OtherInformationStore* otherinformationstore_;

@end
