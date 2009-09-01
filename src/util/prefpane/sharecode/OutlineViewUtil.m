/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineViewUtil.h"

@implementation BUNDLEPREFIX_OutlineViewUtil

+ (void) expandALL:(NSOutlineView *)outlineView
{
  for (;;) {
    bool nochange = true;

    int i = 0;
    for (i = 0; i < [outlineView numberOfRows]; ++i) {
      id item = [outlineView itemAtRow:i];
      if (! [outlineView isExpandable:item]) continue;
      if ([outlineView isItemExpanded:item]) continue;

      [outlineView expandItem:item expandChildren:TRUE];
      nochange = false;
      break;
    }

    if (nochange) break;
  }
}

+ (void) collapseALL:(NSOutlineView *)outlineView
{
  for (;;) {
    bool nochange = true;

    int i = 0;
    for (i = 0; i < [outlineView numberOfRows]; ++i) {
      id item = [outlineView itemAtRow:i];
      if (! [outlineView isExpandable:item]) continue;
      if (! [outlineView isItemExpanded:item]) continue;

      [outlineView collapseItem:item collapseChildren:TRUE];
      nochange = false;
      break;
    }

    if (nochange) break;
  }
}

@end
