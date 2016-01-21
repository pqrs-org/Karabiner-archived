#import "CheckboxOutlineViewDataSource.h"
#import "NotificationKeys.h"
#import "OutlineView.h"
#import "OutlineViewDataSourceNumber.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface OutlineView () {
  NSMutableDictionary* textsHeightCache_;
  dispatch_queue_t textsHeightQueue_;
}

@property(weak) IBOutlet CheckboxOutlineViewDataSource* checkboxOutlineViewDataSource;

@end

@implementation OutlineView

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ setNeedsDisplay:YES];
  });
}

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ setNeedsDisplay:YES];
    if ([showEnabledOnly_ state] == NSOnState) {
      [self filter:self];
    }
  });
}

- (void)observer_ConfigXMLReloaded:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self load:YES];
    [outlineview_ reloadData];
    [self filter:self];
    if (!ischeckbox_) {
      [self expand:self];
    }
  });
}

+ (NSFont*)font {
  return [NSFont systemFontOfSize:[NSFont smallSystemFontSize]];
}

+ (CGFloat)textsHeight:(NSUInteger)lineCount {
  if (lineCount == 0) return 0.0f;

  NSString* line = @"gM\n";
  NSUInteger length = [line length] * lineCount - 1; // skip last '\n'
  NSString* texts = [[NSString string] stringByPaddingToLength:length withString:line startingAtIndex:0];
  NSDictionary* attributes = @{NSFontAttributeName : [OutlineView font]};
  NSSize size = [texts sizeWithAttributes:attributes];
  return size.height;
}

- (id)init {
  self = [super init];

  if (self) {
    textsHeightCache_ = [NSMutableDictionary new];
    textsHeightQueue_ = dispatch_queue_create("org.pqrs.Karabiner.OutlineView.textsHeightQueue_", NULL);

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_PreferencesChanged:)
                                                 name:kPreferencesChangedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigXMLReloaded:)
                                                 name:kConfigXMLReloadedNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)load:(BOOL)force {
  if (ischeckbox_) {
    [self.checkboxOutlineViewDataSource load:force];
  } else {
    [outlineViewDataSourceNumber_ load:force];
  }
}

/* ---------------------------------------------------------------------- */
- (BOOL)isTextCell:(NSTableColumn*)tableColumn item:(id)item {
  if (ischeckbox_) {
    NSString* identifier = item[@"identifier"];
    return (!identifier || [identifier hasPrefix:@"notsave."]);
  } else {
    return NO;
  }
}

- (NSCell*)outlineView:(NSOutlineView*)outlineView dataCellForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  if (!tableColumn) return nil;

  if (ischeckbox_) {
    return nil;
  } else {
    NSString* identifier = item[@"identifier"];
    NSString* columnIdentifier = [tableColumn identifier];

    if (identifier) {
      if ([columnIdentifier isEqualToString:@"value"]) {
        NSTextFieldCell* cell = [NSTextFieldCell new];
        [cell setFont:[OutlineView font]];
        [cell setEditable:YES];
        return cell;

      } else if ([columnIdentifier isEqualToString:@"stepper"]) {
        NSStepperCell* cell = [NSStepperCell new];
        [cell setFont:[OutlineView font]];

        [cell setMinValue:0];
        [cell setMaxValue:1073741824]; // 2^30
        [cell setIncrement:[item[@"step"] intValue]];
        [cell setAutorepeat:NO];
        [cell setValueWraps:NO];
        return cell;
      }
    }

    // ----------------------------------------
    NSCell* cell = [NSCell new];
    [cell setFont:[OutlineView font]];

    if ([columnIdentifier isEqualToString:@"default"]) {
      [cell setEnabled:NO];
    }

    return cell;
  }
}

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item {
  NSNumber* lineCount = item[@"height"];
  __block NSNumber* height = @([outlineView rowHeight]);

  if ([lineCount integerValue] > 1) {
    dispatch_sync(textsHeightQueue_, ^{
      if (textsHeightCache_[lineCount] == nil) {
        textsHeightCache_[lineCount] = @([OutlineView textsHeight:[lineCount unsignedIntegerValue]]);
      }
      // We add [outlineView rowHeight] as vertical margin.
      height = @([textsHeightCache_[lineCount] floatValue] + [outlineView rowHeight]);
    });
  }

  return [height floatValue];
}

/* ---------------------------------------------------------------------- */
- (IBAction)filter:(id)sender {
  if (ischeckbox_) {
    BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
    NSString* string = [searchText_ stringValue];
    [self.checkboxOutlineViewDataSource filterDataSource:isEnabledOnly string:string];

    [outlineview_ reloadData];

    if ([string length] == 0 && isEnabledOnly == NO) {
      [outlineview_ collapseItem:nil collapseChildren:YES];
    } else {
      [outlineview_ expandItem:nil expandChildren:YES];
    }
  }
}

- (IBAction)expand:(id)sender {
  [outlineview_ expandItem:nil expandChildren:YES];
  [outlineview_ setNeedsDisplay:YES];
}

- (IBAction)collapse:(id)sender {
  [outlineview_ collapseItem:nil collapseChildren:YES];
  [outlineview_ setNeedsDisplay:YES];
}

@end
