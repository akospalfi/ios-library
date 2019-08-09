/* Copyright Airship and Contributors */

#import "UAChannel.h"
#import "UAChannelRegistrar+Internal.h"
#import "UATagGroupsRegistrar+Internal.h"
#import "UAPushProviderDelegate.h"
#import "UAUserProviderDelegate+Internal.h"

extern NSString *const UAChannelTagsSettingsKey;
extern NSString *const UAChannelCreationOnForeground;

@interface UAChannel () <UAChannelRegistrarDelegate>

/**
 * The push provider delegate.
 */
@property (nonatomic, strong) id<UAPushProviderDelegate> pushProviderDelegate;

/**
 * The user provider delegate;
 */
@property (nonatomic, strong) id<UAUserProviderDelegate> userProviderDelegate;

/**
 * Flag indicating app is running in the foreground
 */
@property (nonatomic, assign) BOOL isForegrounded;

/**
 * Allows disabling channel registration before a channel is created.  Channel registration will resume
 * when this flag is set to `YES`.
 *
 * Set this to `NO` to disable channel registration. Defaults to `YES`.
 */
@property (nonatomic, assign, getter=isChannelCreationEnabled) BOOL channelCreationEnabled;

+ (instancetype)channelWithDataStore:(UAPreferenceDataStore *)dataStore
                              config:(UARuntimeConfig *)config
                  notificationCenter:(NSNotificationCenter *)notificationCenter
                    channelRegistrar:(UAChannelRegistrar *)channelRegistrar
                  tagGroupsRegistrar:(UATagGroupsRegistrar *)tagGroupsRegistrar;

/**
 * Called when the application becomes active. Triggers registration updates.
 */
- (void)applicationDidBecomeActive;

/**
 * Called when the application enters the background. Used to clear a flag set on foreground
 * to prevent double registration on app init.
 */
- (void)applicationDidEnterBackground;

/**
 * Registers or updates the current registration with an API call. If push notifications are
 * not enabled, this unregisters the device token.
 *
 * Observe NSNotificationCenterEvents such as UAChannelCreatedEvent, UAChannelUpdatedEvent and UAChannelRegistrationFailedEvent
 * to receive success and failure callbacks.
 *
 * @param forcefully Tells the device api client to do any device api call forcefully.
 */
- (void)updateRegistrationForcefully:(BOOL)forcefully;

/**
 * Called to update the tag groups for the current channel.
 */
- (void)updateChannelTagGroups;

/**
 * Removes the existing channel and causes the registrar to create a new channel on next registration.
 */
- (void)reset;

@end

