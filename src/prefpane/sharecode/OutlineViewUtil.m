// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "OutlineViewUtil.h"

@implementation org_pqrs_OutlineViewUtil

+ (void) intelligentExpand:(NSOutlineView *)outlineView delegater:(id)delegater
{
  for (;;) {
    bool nochange = true;

    int i = 0;
    for (i = 0; i < [outlineView numberOfRows]; ++i) {
      id item = [outlineView itemAtRow:i];
      if (! [outlineView isExpandable:item]) continue;

      if ([delegater outlineView:outlineView shouldCollapseItem:item]) {
        // collapse item
        if (! [outlineView isItemExpanded:item]) continue;

        [outlineView collapseItem:item];
        nochange = false;
        break;

      } else {
        // expand item
        if ([outlineView isItemExpanded:item]) continue;

        [outlineView expandItem:item];
        nochange = false;
        break;
      }
    }

    if (nochange) break;
  }
}

@end
