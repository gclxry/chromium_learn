// Copyright 2012 Google Inc.
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

//
// Factory for the invalidation client library.

#ifndef GOOGLE_CACHEINVALIDATION_INCLUDE_INVALIDATION_CLIENT_FACTORY_H_
#define GOOGLE_CACHEINVALIDATION_INCLUDE_INVALIDATION_CLIENT_FACTORY_H_

#include <string>

#include "google/cacheinvalidation/include/invalidation-listener.h"
#include "google/cacheinvalidation/include/system-resources.h"
#include "google/cacheinvalidation/deps/stl-namespace.h"

namespace invalidation {

using INVALIDATION_STL_NAMESPACE::string;

/* Constructs an invalidation client library instance with a default
 * configuration. Caller owns returned space.
 *
 * Arguments:
 *   resources SystemResources to use for logging, scheduling, persistence,
 *       and network connectivity
 *   client_type client type code as assigned by the notification system's
 *       backend
 *   client_name id/name of the client in the application's own naming scheme
 *   application_name name of the application using the library (for
 *       debugging/monitoring)
 *   listener callback object for invalidation events
 */
InvalidationClient* CreateInvalidationClient(
    SystemResources* resources,
    int client_type,
    const string& client_name,
    const string& application_name,
    InvalidationListener* listener);

/* Constructs an invalidation client library instance with a configuration
 * initialized for testing. Caller owns returned space.
 *
 * Arguments:
 *   resources SystemResources to use for logging, scheduling, persistence,
 *       and network connectivity
 *   client_type client type code as assigned by the notification system's
 *       backend
 *   client_name id/name of the client in the application's own naming scheme
 *   application_name name of the application using the library (for
 *       debugging/monitoring)
 *   listener callback object for invalidation events
 */
InvalidationClient* CreateInvalidationClientForTest(
    SystemResources* resources,
    int client_type,
    const string& client_name,
    const string& application_name,
    InvalidationListener* listener);

}  // namespace invalidation

#endif  // GOOGLE_CACHEINVALIDATION_INCLUDE_INVALIDATION_CLIENT_FACTORY_H_
