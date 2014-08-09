// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_content_browser_client.h"

#include "base/command_line.h"
#include "base/file_util.h"
#include "base/path_service.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/resource_dispatcher_host.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
//#include "content/shell/geolocation/shell_access_token_store.h"
//#include "content/shell/shell.h"
//#include "content/simple/simple_browser_context.h"
#include "content/simple/simple_browser_main_parts.h"
//#include "content/shell/shell_devtools_delegate.h"
//#include "content/shell/shell_message_filter.h"
//#include "content/shell/shell_messages.h"
//#include "content/shell/shell_quota_permission_context.h"
//#include "content/shell/shell_resource_dispatcher_host_delegate.h"
#include "content/simple/simple_switches.h"
//#include "content/shell/shell_web_contents_view_delegate_creator.h"
//#include "content/shell/webkit_test_controller.h"
//#include "content/shell/webkit_test_helpers.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/webpreferences.h"


namespace content {

namespace {

SimpleContentBrowserClient* g_browser_client;

}  // namespace

SimpleContentBrowserClient* SimpleContentBrowserClient::Get() {
  return g_browser_client;
}

SimpleContentBrowserClient::SimpleContentBrowserClient()
  : hyphen_dictionary_file_(base::kInvalidPlatformFileValue),
    simple_browser_main_parts_(NULL) {
      DCHECK(!g_browser_client);
      g_browser_client = this;
      //if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
      //  return;
      //webkit_source_dir_ = GetWebKitRootDirFilePath();
      //base::FilePath dictionary_file_path = base::MakeAbsoluteFilePath(
      //  GetChromiumRootDirFilePath().Append(
      //  FILE_PATH_LITERAL("third_party/hyphen/hyph_en_US.dic")));
      //hyphen_dictionary_file_ = base::CreatePlatformFile(dictionary_file_path,
      //  base::PLATFORM_FILE_READ |
      //  base::PLATFORM_FILE_OPEN,
      //  NULL,
      //  NULL);
}

SimpleContentBrowserClient::~SimpleContentBrowserClient() {
  g_browser_client = NULL;
}

BrowserMainParts* SimpleContentBrowserClient::CreateBrowserMainParts(
  const MainFunctionParams& parameters) {
    simple_browser_main_parts_ = new SimpleBrowserMainParts(parameters);
    return simple_browser_main_parts_;
}

//void SimpleContentBrowserClient::RenderProcessHostCreated(
//  RenderProcessHost* host) {
    //if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
    //  return;
    //host->GetChannel()->AddFilter(new ShellMessageFilter(
    //  host->GetID(),
    //  BrowserContext::GetDefaultStoragePartition(browser_context())
    //  ->GetDatabaseTracker(),
    //  BrowserContext::GetDefaultStoragePartition(browser_context())
    //  ->GetQuotaManager()));
    //host->Send(new ShellViewMsg_SetWebKitSourceDir(webkit_source_dir_));
    //registrar_.Add(this,
    //  NOTIFICATION_RENDERER_PROCESS_CREATED,
    //  Source<RenderProcessHost>(host));
//}

//net::URLRequestContextGetter* SimpleContentBrowserClient::CreateRequestContext(
//  BrowserContext* content_browser_context,
//  ProtocolHandlerMap* protocol_handlers) {
//    SimpleBrowserContext* shell_browser_context =
//      SimpleBrowserContextForBrowserContext(content_browser_context);
//    return shell_browser_context->CreateRequestContext(protocol_handlers);
//}
//
//net::URLRequestContextGetter*
//  SimpleContentBrowserClient::CreateRequestContextForStoragePartition(
//  BrowserContext* content_browser_context,
//  const base::FilePath& partition_path,
//  bool in_memory,
//  ProtocolHandlerMap* protocol_handlers) {
//    SimpleBrowserContext* shell_browser_context =
//      SimpleBrowserContextForBrowserContext(content_browser_context);
//    return shell_browser_context->CreateRequestContextForStoragePartition(
//      partition_path, in_memory, protocol_handlers);
//}
//
//void SimpleContentBrowserClient::AppendExtraCommandLineSwitches(
//  CommandLine* command_line, int child_process_id) {
//    if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
//      command_line->AppendSwitch(switches::kDumpRenderTree);
//}

//void SimpleContentBrowserClient::OverrideWebkitPrefs(
//  RenderViewHost* render_view_host,
//  const GURL& url,
//  webkit_glue::WebPreferences* prefs) {
//    if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
//      return;
//    WebKitTestController::Get()->OverrideWebkitPrefs(prefs);
//}

//void SimpleContentBrowserClient::ResourceDispatcherHostCreated() {
  //resource_dispatcher_host_delegate_.reset(
  //  new ShellResourceDispatcherHostDelegate());
  //ResourceDispatcherHost::Get()->SetDelegate(
  //  resource_dispatcher_host_delegate_.get());
//}

//std::string SimpleContentBrowserClient::GetDefaultDownloadName() {
//  return "download";
//}
//
//bool SimpleContentBrowserClient::SupportsBrowserPlugin(
//  content::BrowserContext* browser_context, const GURL& url) {
//    return CommandLine::ForCurrentProcess()->HasSwitch(
//      switches::kEnableBrowserPluginForAllViewTypes);
//}

//WebContentsViewDelegate* SimpleContentBrowserClient::GetWebContentsViewDelegate(
//    WebContents* web_contents) {
//#if !defined(USE_AURA)
//  return CreateShellWebContentsViewDelegate(web_contents);
//#else
//  return NULL;
//#endif
//}
//
//QuotaPermissionContext*
//SimpleContentBrowserClient::CreateQuotaPermissionContext() {
//  return new ShellQuotaPermissionContext();
//}


void SimpleContentBrowserClient::Observe(int type,
  const NotificationSource& source,
  const NotificationDetails& details) 
{
    switch (type) 
    {
      case NOTIFICATION_RENDERER_PROCESS_CREATED: 
      {
        registrar_.Remove(this,
          NOTIFICATION_RENDERER_PROCESS_CREATED,
          source);
        if (hyphen_dictionary_file_ != base::kInvalidPlatformFileValue) 
        {
          //RenderProcessHost* host = Source<RenderProcessHost>(source).ptr();
          //IPC::PlatformFileForTransit file = IPC::GetFileHandleForProcess(
          //  hyphen_dictionary_file_, host->GetHandle(), false);
          //host->Send(new ShellViewMsg_LoadHyphenDictionary(file));
        }
        break;
      }

    default:
      NOTREACHED();
    }
}


//SimpleBrowserContext* SimpleContentBrowserClient::browser_context() {
//  return simple_browser_main_parts_->browser_context();
//}
//
//SimpleBrowserContext*
//  SimpleContentBrowserClient::off_the_record_browser_context() {
//    return simple_browser_main_parts_->off_the_record_browser_context();
//}
//
//AccessTokenStore* SimpleContentBrowserClient::CreateAccessTokenStore() {
//  return new ShellAccessTokenStore(browser_context()->GetRequestContext());
//}

//SimpleBrowserContext*
//  SimpleContentBrowserClient::SimpleBrowserContextForBrowserContext(
//  BrowserContext* content_browser_context) {
//    if (content_browser_context == browser_context())
//      return browser_context();
//    DCHECK_EQ(content_browser_context, off_the_record_browser_context());
//    return off_the_record_browser_context();
//}

}  // namespace content
