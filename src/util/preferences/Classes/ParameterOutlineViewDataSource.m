#import "ParameterOutlineViewDataSource.h"
#import "ParameterTree.h"
#import "ServerClient.h"

@interface ParameterOutlineViewDataSource ()

@property(weak) IBOutlet ServerClient* client;
@property ParameterTree* dataSource;

@end

@implementation ParameterOutlineViewDataSource

- (void)setup {
  self.dataSource = [self.client parameterTree];
}

- (void)clear {
  self.dataSource = [ParameterTree new];
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  ParameterTree* tree = (ParameterTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
  ParameterTree* tree = (ParameterTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;

  if ((NSUInteger)(index) >= [a count]) return nil;
  return a[index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  ParameterTree* tree = (ParameterTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;
  return [a count] > 0;
}

@end
