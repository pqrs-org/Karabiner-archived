#import "ParameterOutlineViewDataSource.h"
#import "PreferencesWindowController.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"
#import "SharedXMLCompilerTree.h"
#import "XMLCompiler.h"

@interface ParameterOutlineViewDataSource ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property SharedXMLCompilerTree* dataSource;

@end

@implementation ParameterOutlineViewDataSource

- (void)load:(BOOL)force {
  if (force) {
    if (self.dataSource) {
      self.dataSource = nil;
    }
  }

  if (!self.dataSource) {
    self.dataSource = self.preferencesWindowController.serverObjects.serverForUserspace.sharedParameterTree;
  }
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  [self load:NO];

  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
  [self load:NO];

  // ----------------------------------------
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;

  if ((NSUInteger)(index) >= [a count]) return nil;
  return a[index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;
  return [a count] > 0;
}

@end
