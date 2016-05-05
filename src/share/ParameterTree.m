#import "ParameterTree.h"

@interface ParameterItem ()

@property(copy, readwrite) NSString* name;
@property(copy, readwrite) NSString* identifier;
@property(readwrite) NSInteger defaultValue;
@property(readwrite) NSInteger step;
@property(copy, readwrite) NSString* baseUnit;

@end

@implementation ParameterItem

- (instancetype)initWithName:(const char*)name
                  identifier:(const char*)identifier
                defaultValue:(int)defaultValue
                        step:(int)step
                    baseUnit:(const char*)baseUnit {
  self = [super init];

  if (self) {
    self.name = name ? @(name) : @"";
    self.identifier = identifier ? @(identifier) : @"";
    self.defaultValue = defaultValue;
    self.step = step;
    self.baseUnit = baseUnit ? @(baseUnit) : @"";
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
    self.name = [decoder decodeObjectForKey:@"name"];
    self.identifier = [decoder decodeObjectForKey:@"identifier"];
    self.defaultValue = [decoder decodeIntegerForKey:@"defaultValue"];
    self.step = [decoder decodeIntegerForKey:@"step"];
    self.baseUnit = [decoder decodeObjectForKey:@"baseUnit"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.name forKey:@"name"];
  [encoder encodeObject:self.identifier forKey:@"identifier"];
  [encoder encodeInteger:self.defaultValue forKey:@"defaultValue"];
  [encoder encodeInteger:self.step forKey:@"step"];
  [encoder encodeObject:self.baseUnit forKey:@"baseUnit"];
}

#pragma mark - NSCoping

- (id)copyWithZone:(NSZone*)zone {
  ParameterItem* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.name = [self.name copyWithZone:zone];
    obj.identifier = [self.identifier copyWithZone:zone];
    obj.defaultValue = self.defaultValue;
    obj.step = self.step;
    obj.baseUnit = [self.baseUnit copyWithZone:zone];
  }
  return obj;
}

@end

@interface ParameterTree ()

@property(readwrite) ParameterItem* node;
@property(copy, readwrite) NSArray* children;

@end

@implementation ParameterTree

- (instancetype)initWithItem:(ParameterItem*)node
                    children:(NSArray*)children {
  self = [super init];

  if (self) {
    self.node = node;
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
    self.node = [decoder decodeObjectForKey:@"node"];
    self.children = [decoder decodeObjectForKey:@"children"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.node forKey:@"node"];
  [encoder encodeObject:self.children forKey:@"children"];
}

#pragma mark - NSCoping

- (id)copyWithZone:(NSZone*)zone {
  ParameterTree* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.node = [self.node copyWithZone:zone];
    obj.children = [self.children copyWithZone:zone];
  }
  return obj;
}

@end
