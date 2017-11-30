/* Copyright 2017 Urban Airship and Contributors */

#import <UIKit/UIKit.h>
#import "UALegacyInAppMessage.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * View class for in-app messages.
 */
@interface UALegacyInAppMessageView : UIView

///---------------------------------------------------------------------------------------
/// @name In App Message View Properties
///---------------------------------------------------------------------------------------

/**
 * The "tab" widget indicating swipability.
 */
@property(nonatomic, readonly) IBOutlet UIView *tab;

/**
 * The message label displaying message alert content.
 */
@property(nonatomic, readonly) IBOutlet UILabel *messageLabel;

/**
 * Button one.
 */
@property(nonatomic, readonly, nullable) IBOutlet UIButton *button1;

/**
 * Button two.
 */
@property(nonatomic, readonly, nullable) IBOutlet UIButton *button2;

///---------------------------------------------------------------------------------------
/// @name In App Message View Initialization
///---------------------------------------------------------------------------------------

/**
 * UALegacyInAppMessageView initializer.
 * @param position A `UALegacyInAppMessagePosition` value, indicating screen position.
 * @param numberOfButtons The number of buttons to display (0-2).
 */
- (instancetype)initWithPosition:(UALegacyInAppMessagePosition)position numberOfButtons:(NSUInteger)numberOfButtons;

@end

NS_ASSUME_NONNULL_END

