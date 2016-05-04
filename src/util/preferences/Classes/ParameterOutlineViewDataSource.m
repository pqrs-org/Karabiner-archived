#import "ParameterOutlineViewDataSource.h"
#import "ServerClient.h"
#import "SharedXMLCompilerTree.h"

@interface ParameterOutlineViewDataSource ()

@property(weak) IBOutlet ServerClient* client;
@property SharedXMLCompilerTree* dataSource;

@end

@implementation ParameterOutlineViewDataSource

- (void)setup {
  self.dataSource = [self.client.proxy sharedParameterTree];
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
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
