#import "ParameterOutlineViewDataSource.h"
#import "PreferencesManager.h"
#import "ServerObjects.h"
#import "XMLCompiler.h"

@interface ParameterOutlineViewDataSource ()
@property(weak) IBOutlet ServerObjects* serverObjects;
@property XMLCompilerTree* dataSource;
@end

@implementation ParameterOutlineViewDataSource

- (void)load:(BOOL)force {
  if (force) {
    if (self.dataSource) {
      self.dataSource = nil;
    }
  }

  if (!self.dataSource) {
    self.dataSource = self.serverObjects.xmlCompiler.preferencepane_parameter;
  }
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  [self load:NO];

  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
  [self load:NO];

  // ----------------------------------------
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;

  if ((NSUInteger)(index) >= [a count]) return nil;
  return a[index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;
  return [a count] > 0;
}

@end
