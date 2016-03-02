#import "OtherInformationStore.h"

@interface OtherInformationStore()

@property(weak) IBOutlet NSTextField* label_languagecode;
@property(weak) IBOutlet NSTextField* label_inputsourceid;
@property(weak) IBOutlet NSTextField* label_inputmodeid;
@property(copy) NSString* languagecode;
@property(copy) NSString* inputsourceid;
@property(copy) NSString* inputmodeid;

@end

@implementation OtherInformationStore

- (IBAction)copy:(id)sender {
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [NSMutableString new];

  [string appendFormat:@"Language Code: %@\n", self.languagecode];
  [string appendFormat:@"Input Source ID: %@\n", self.inputsourceid];
  [string appendFormat:@"Input Mode ID: %@\n", self.inputmodeid];

  [pboard clearContents];
  [pboard writeObjects:@[ string ]];
}

- (void)setLanguageCode:(NSString*)value {
  if (!value) {
    value = @"---";
  }

  self.languagecode = value;
  [self.label_languagecode setStringValue:value];
}

- (void)setInputSourceID:(NSString*)value {
  if (!value) {
    value = @"---";
  }

  self.inputsourceid = value;
  [self.label_inputsourceid setStringValue:value];
}

- (void)setInputModeID:(NSString*)value {
  if (!value) {
    value = @"---";
  }

  self.inputmodeid = value;
  [self.label_inputmodeid setStringValue:value];
}

@end
