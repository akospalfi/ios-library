/* Copyright Airship and Contributors */

#import "UAAccengage+Internal.h"
#import "UAActionRunner.h"
#import "UAAccengagePayload.h"
#import "UAChannelRegistrationPayload+Internal.h"
#import "UAExtendableChannelRegistration.h"
#import "UAJSONSerialization.h"

@implementation UAAccengage

- (instancetype)initWithDataStore:(UAPreferenceDataStore *)dataStore
                          channel:(UAChannel<UAExtendableChannelRegistration> *)channel
                             push:(UAPush *)push
                        analytics:(UAAnalytics *)analytics {
    self = [super initWithDataStore:dataStore];
    if (self) {
        UA_WEAKIFY(self);
        [channel addChannelExtenderBlock:^(UAChannelRegistrationPayload *payload, UAChannelRegistrationExtenderCompletionHandler completionHandler) {
            UA_STRONGIFY(self);
            [self extendChannelRegistrationPayload:payload completionHandler:completionHandler];
        }];
    }
    return self;
}

+ (instancetype)accengageWithDataStore:(UAPreferenceDataStore *)dataStore
                               channel:(UAChannel<UAExtendableChannelRegistration> *)channel
                                  push:(UAPush *)push
                             analytics:(UAAnalytics *)analytics {

    return [[self alloc] initWithDataStore:dataStore channel:channel push:push analytics:analytics];
}

-(void)receivedNotificationResponse:(UANotificationResponse *)response
                  completionHandler:(void (^)(void))completionHandler {
    // check for accengage push response, handle actions
    NSDictionary *notificationInfo = response.notificationContent.notificationInfo;
    
    UAAccengagePayload *payload = [UAAccengagePayload payloadWithDictionary:notificationInfo];
    
    if (!payload.identifier) {
        // not an Accengage push
        completionHandler();
        return;
    }
      
    if (payload.url) {
        if (payload.hasExternalURLAction) {
            [UAActionRunner runActionWithName:@"open_external_url_action"
                                        value:payload.url
                                    situation:UASituationLaunchedFromPush];
        } else {
            [UAActionRunner runActionWithName:@"landing_page_action"
                                        value:payload.url
                                    situation:UASituationLaunchedFromPush];
        }
    }
        
    if (![response.actionIdentifier isEqualToString:UANotificationDismissActionIdentifier] && ![response.actionIdentifier isEqualToString:UANotificationDefaultActionIdentifier] &&
        payload.buttons) {
        for (UAAccengageButton *button in payload.buttons) {
            if ([button.identifier isEqualToString:response.actionIdentifier]) {
                if ([button.actionType isEqualToString:UAAccengageButtonBrowserAction]) {
                        [UAActionRunner runActionWithName:@"open_external_url_action"
                                                    value:button.url
                                                situation:UASituationForegroundInteractiveButton];
                } else if ([button.actionType isEqualToString:UAAccengageButtonWebviewAction]) {
                        [UAActionRunner runActionWithName:@"landing_page_action"
                                                    value:button.url
                                                situation:UASituationForegroundInteractiveButton];
                }
            }
        }
    }
    
    completionHandler();
}

#pragma mark -
#pragma mark Channel Registration Events

- (void)extendChannelRegistrationPayload:(UAChannelRegistrationPayload *)payload
                       completionHandler:(UAChannelRegistrationExtenderCompletionHandler)completionHandler {
    
    // get the Accengage ID and set it as an identity hint
    NSString *accengageDeviceID = UIDevice.currentDevice.identifierForVendor.UUIDString;
    
    payload.accengageDeviceID = accengageDeviceID;
    
    completionHandler(payload);
}

@end
