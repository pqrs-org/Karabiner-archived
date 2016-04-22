#import "CheckboxTree.h"

static NSInteger itemId_;
static dispatch_queue_t itemIdQueue_;

@interface SharedCheckboxItem ()

@property(readwrite) NSNumber* id;
@property(copy, readwrite) NSString* name;
@property(copy, readwrite) NSString* style;
@property(copy, readwrite) NSString* identifier;
@property(readwrite) NSInteger childrenCount;
@property(readwrite) BOOL needsShowCheckbox;

@end

@implementation SharedCheckboxItem

+ (void)initialize {
  itemId_ = 0;
  itemIdQueue_ = dispatch_queue_create("org.pqrs.Karabiner.SharedCheckboxTree.itemIdQueue_", NULL);
}

- (instancetype)initWithName:(NSString*)name
                       style:(NSString*)style
                  identifier:(NSString*)identifier
               childrenCount:(NSInteger)childrenCount
           needsShowCheckbox:(BOOL)needsShowCheckbox {
  self = [super init];

  if (self) {
    dispatch_sync(itemIdQueue_, ^{
      ++itemId_;
      self.id = @(itemId_);
    });

    self.name = name;
    self.style = style;
    self.identifier = identifier;
    self.childrenCount = childrenCount;
    self.needsShowCheckbox = needsShowCheckbox;
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
    self.name = [decoder decodeObjectForKey:@"name"];
    self.style = [decoder decodeObjectForKey:@"style"];
    self.identifier = [decoder decodeObjectForKey:@"identifier"];
    self.childrenCount = [decoder decodeObjectForKey:@"childrenCount"];
    self.needsShowCheckbox = [decoder decodeIntForKey:@"needsShowCheckbox"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.id forKey:@"id"];
  [encoder encodeObject:self.name forKey:@"name"];
  [encoder encodeObject:self.style forKey:@"style"];
  [encoder encodeObject:self.identifier forKey:@"identifier"];
  [encoder encodeInteger:self.childrenCount forKey:@"childrenCount"];
  [encoder encodeBool:self.needsShowCheckbox forKey:@"needsShowCheckbox"];
}

- (id)copyWithZone:(NSZone*)zone {
  SharedCheckboxItem* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.id = [self.id copyWithZone:zone];
    obj.name = [self.name copyWithZone:zone];
    obj.style = [self.style copyWithZone:zone];
    obj.identifier = [self.identifier copyWithZone:zone];
    obj.childrenCount = self.childrenCount;
    obj.needsShowCheckbox = self.needsShowCheckbox;
  }
  return obj;
}

@end

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
