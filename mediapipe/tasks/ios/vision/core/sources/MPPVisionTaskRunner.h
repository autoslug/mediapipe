// Copyright 2023 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "mediapipe/tasks/ios/core/sources/MPPTaskRunner.h"
#import "mediapipe/tasks/ios/vision/core/sources/MPPRunningMode.h"

#include "mediapipe/framework/formats/rect.pb.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * This class is used to create and call appropriate methods on the C++ Task Runner to initialize,
 * execute and terminate any MediaPipe vision task.
 */
@interface MPPVisionTaskRunner : MPPTaskRunner

/**
 * Initializes a new `MPPVisionTaskRunner` with the MediaPipe calculator config proto running mode
 * and packetsCallback.
 * Make sure that the packets callback is set properly based on the vision task's running mode.
 * In case of live stream running mode, a C++ packets callback that is intended to deliver inference
 * results must be provided. In case of image or video running mode, packets callback must be set to
 * nil.
 *
 * @param graphConfig A MediaPipe calculator config proto.
 * @param runningMode MediaPipe vision task running mode.
 * @param packetsCallback An optional C++ callback function that takes a list of output packets as
 * the input argument. If provided, the callback must in turn call the block provided by the user in
 * the appropriate task options. Make sure that the packets callback is set properly based on the
 * vision task's running mode. In case of live stream running mode, a C++ packets callback that is
 * intended to deliver inference results must be provided. In case of image or video running mode,
 * packets callback must be set to nil.
 *
 * @param error Pointer to the memory location where errors if any should be saved. If @c NULL, no
 * error will be saved.
 *
 * @return An instance of `MPPVisionTaskRunner` initialized to the given MediaPipe calculator config
 * proto, running mode and packets callback.
 */
- (nullable instancetype)initWithCalculatorGraphConfig:(mediapipe::CalculatorGraphConfig)graphConfig
                                           runningMode:(MPPRunningMode)runningMode
                                       packetsCallback:
                                           (mediapipe::tasks::core::PacketsCallback)packetsCallback
                                                 error:(NSError **)error NS_DESIGNATED_INITIALIZER;

/**
 * Creates a `NormalizedRect` from region of interest and image orientation, performing
 * sanity checks on-the-fly. If the input region of interest equals `CGRectZero`, returns a default
 * `NormalizedRect` covering the whole image with rotation set according `imageOrientation`. If
 * `roiAllowed` is NO, an error will be returned if the input region of interest is not equal to
 * `CGRectZero`.
 *
 * @param roi A `CGRect` specifying the region of interest. If the input region of interest equals
 * `CGRectZero`, the returned `NormalizedRect` covers the whole image. Make sure that `roi` equals
 * `CGRectZero` if `roiAllowed` is NO. Otherwise an error will be returned.
 * @param imageOrientation A `UIImageOrientation` indicating the rotation to be applied to the
 * image. The resulting `NormalizedRect` will convert the `imageOrientation` to degrees clockwise.
 * @param roiAllowed Indicates if the `roi` field is allowed to be a value other than `CGRectZero`.
 *
 * @param error Pointer to the memory location where errors if any should be
 * saved. If @c NULL, no error will be saved.
 *
 * @return An optional `NormalizedRect` from the given region of interest and image orientation.
 */
- (std::optional<mediapipe::NormalizedRect>)
    normalizedRectFromRegionOfInterest:(CGRect)roi
                      imageOrientation:(UIImageOrientation)imageOrientation
                            roiAllowed:(BOOL)roiAllowed
                                 error:(NSError **)error;

/**
 * A synchronous method to invoke the  C++ task runner to process single image inputs. The call
 * blocks the current thread until a failure status or a successful result is returned.
 *
 * @param packetMap A `PackeMap` containing pairs of input stream name and data packet.
 * @param error Pointer to the memory location where errors if any should be
 * saved. If @c NULL, no error will be saved.
 *
 * @return An optional `PacketMap` containing pairs of output stream name and data packet.
 */
- (std::optional<mediapipe::tasks::core::PacketMap>)
    processImagePacketMap:(const mediapipe::tasks::core::PacketMap &)packetMap
                    error:(NSError **)error;

/**
 * A synchronous method to invoke the  C++ task runner to process continuous video frames. The call
 * blocks the current thread until a failure status or a successful result is returned.
 *
 * @param packetMap A `PackeMap` containing pairs of input stream name and data packet.
 * @param error Pointer to the memory location where errors if any should be
 * saved. If @c NULL, no error will be saved.
 *
 * @return An optional `PacketMap` containing pairs of output stream name and data packet.
 */
- (std::optional<mediapipe::tasks::core::PacketMap>)
    processVideoFramePacketMap:(const mediapipe::tasks::core::PacketMap &)packetMap
                         error:(NSError **)error;

/**
 * An asynchronous method to send live stream data to the C++ task runner. The call blocks the
 * current thread until a failure status or a successful result is returned. The results will be
 * available in the user-defined `packetsCallback` that was provided during initialization of the
 * `MPPVisionTaskRunner`.
 *
 * @param packetMap A `PackeMap` containing pairs of input stream name and data packet.
 * @param error Pointer to the memory location where errors if any should be
 * saved. If @c NULL, no error will be saved.
 *
 * @return A `BOOL` indicating if the live stream data was sent to the C++ task runner successfully.
 * Please note that any errors during processing of the live stream packet will only be available in
 * the user-defined `packetsCallback` that was provided during initialization of the
 * `MPPVisionTaskRunner`.
 */
- (BOOL)processLiveStreamPacketMap:(const mediapipe::tasks::core::PacketMap &)packetMap
                             error:(NSError **)error;

- (instancetype)init NS_UNAVAILABLE;

+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
