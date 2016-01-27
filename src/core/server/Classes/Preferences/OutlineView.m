#import "CheckboxOutlineViewDataSource.h"
#import "NotificationKeys.h"
#import "OutlineView.h"
#import "OutlineViewDataSourceNumber.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface OutlineView ()
@property(weak) IBOutlet CheckboxOutlineViewDataSource* checkboxOutlineViewDataSource;
@end

@implementation OutlineView

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ reloadData];
  });
}

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ reloadData];
    if ([showEnabledOnly_ state] == NSOnState) {
      [self.checkboxOutlineViewDataSource clearFilterCondition];
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

- (id)init {
  self = [super init];

  if (self) {
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

/* ---------------------------------------------------------------------- */
- (IBAction)filter:(id)sender {
  if (ischeckbox_) {
    BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
    NSString* string = [searchText_ stringValue];
    if ([self.checkboxOutlineViewDataSource filterDataSource:isEnabledOnly string:string]) {
      [outlineview_ reloadData];

      if ([string length] == 0 && isEnabledOnly == NO) {
        [outlineview_ collapseItem:nil collapseChildren:YES];
      } else {
        [outlineview_ expandItem:nil expandChildren:YES];
      }
    }
  }
}

- (IBAction)expand:(id)sender {
  [outlineview_ expandItem:nil expandChildren:YES];
}

- (IBAction)collapse:(id)sender {
  [outlineview_ collapseItem:nil collapseChildren:YES];
}

@end
