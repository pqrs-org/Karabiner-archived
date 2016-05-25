#import "AppQueue.h"

@interface AppQueue ()

@property NSMutableArray* queue;
@property(weak) IBOutlet NSTableView* view;

@end

@implementation AppQueue

enum {
  MAXNUM = 50,
};

- (instancetype)init {
  self = [super init];

  if (self) {
    _queue = [NSMutableArray new];
  }

  return self;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  if ([self.queue count] == 0) {
    return 1;
  }

  return [self.queue count];
}

- (id)tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex {
  NSString* identifier = [aTableColumn identifier];

  if ([self.queue count] == 0) {
    return @"Please activate the other applications.";
  }

  NSDictionary* dict = self.queue[([self.queue count] - 1 - rowIndex)];
  return dict[identifier];
}

- (void)refresh {
  [self.view reloadData];
  [self.view scrollRowToVisible:([self.queue count] - 1)];
}

- (void)push:(NSDictionary*)dictionary {
  NSString* bundleIdentifier = dictionary[@"BundleIdentifier"];
  if (!bundleIdentifier) {
    bundleIdentifier = @"";
  }

  NSString* windowName = dictionary[@"WindowName"];
  if (!windowName) {
    windowName = @"";
  }

  NSString* role = dictionary[@"UIElementRole"];
  if (!role) {
    role = @"";
  }

  NSDictionary* dict = @{ @"BundleIdentifier" : bundleIdentifier,
                          @"WindowName" : windowName,
                          @"UIElementRole" : role,
                          @"date" : [[NSDate date] description] };

  [self.queue insertObject:dict atIndex:0];
  if ([self.queue count] > MAXNUM) {
    [self.queue removeLastObject];
  }
  [self refresh];
}

- (IBAction)clear:(id)sender {
  [self.queue removeAllObjects];
  [self refresh];
}

- (IBAction)copy:(id)sender {
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [NSMutableString new];

  for (NSUInteger i = 0; i < [self.queue count]; ++i) {
    NSDictionary* dict = self.queue[([self.queue count] - 1 - i)];

    [string appendFormat:@"BundleIdentifier: %@\n", dict[@"BundleIdentifier"]];
    [string appendFormat:@"WindowName:       %@\n", dict[@"WindowName"]];
    [string appendFormat:@"UIElementRole:    %@\n", dict[@"UIElementRole"]];
    [string appendString:@"\n"];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:@[ string ]];
  }
}

@end
