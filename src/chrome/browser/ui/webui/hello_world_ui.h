#ifndef CHROME_BROWSER_UI_WEBUI_HELLO_WORLD_UI_H_
#define CHROME_BROWSER_UI_WEBUI_HELLO_WORLD_UI_H_
#pragma once

#include "content/public/browser/web_ui_controller.h"

namespace base {
	class ListValue;
}  // namespace base

// The WebUI for chrome://hello-world
class HelloWorldUI : public content::WebUIController {
 public:
  explicit HelloWorldUI(content::WebUI* web_ui);
  virtual ~HelloWorldUI();

 private:
	 // Add two numbers together using integer arithmetic.
	 void AddNumbers(const base::ListValue* args);
  DISALLOW_COPY_AND_ASSIGN(HelloWorldUI);
};

#endif  // CHROME_BROWSER_UI_WEBUI_HELLO_WORLD_UI_H_