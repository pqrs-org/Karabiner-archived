#import "SharedCheckboxTree.h"

@implementation SharedCheckboxTree

- (instancetype)initWithId:(NSNumber*)id
                  children:(NSArray*)children {
  self = [super init];

  if (self) {
    self.id = id;
    self.children = children;
  }

  return self;
}

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    self.id = [decoder decodeObjectForKey:@"id"];
    self.children = [decoder decodeObjectForKey:@"children"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.id forKey:@"id"];
  [encoder encodeObject:self.children forKey:@"children"];
}

#pragma mark - NSCoping

- (id)copyWithZone:(NSZone*)zone {
  SharedCheckboxTree* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.id = [self.id copyWithZone:zone];
    obj.children = [self.children copyWithZone:zone];
  }
  return obj;
}

@end
