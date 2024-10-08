/* START OF widget.h -----------------------------------------------------------
 *
 *	Base widget API.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */

#ifndef _DW_WIDGET_H_
#define _DW_WIDGET_H_

#include "types.h"

#include "widget.h"

#include <SDL.h>

#define EG_WIDGET_MAX_NAME_SIZE                         256
#define EG_WIDGET_MAX_STRING_VALUE_SIZE                 1024

struct tagEG_Widget;

/* Widget callbacks:
 */
typedef EG_BOOL (*EG_Callback_Paint)(struct tagEG_Widget*, SDL_Rect);
typedef EG_BOOL	(*EG_Callback_SDL_Event)(struct tagEG_Widget*, SDL_Event*);
typedef EG_BOOL	(*EG_Callback_Visible)(struct tagEG_Widget*, EG_BOOL);
typedef EG_BOOL	(*EG_Callback_Enabled)(struct tagEG_Widget*, EG_BOOL);
typedef EG_BOOL	(*EG_Callback_GotFocus)(struct tagEG_Widget*);
typedef EG_BOOL	(*EG_Callback_Stopped)(struct tagEG_Widget*, EG_BOOL);
typedef void	(*EG_Callback_LostFocus)(struct tagEG_Widget*);
typedef void	(*EG_Callback_Attach)(struct tagEG_Widget*, EG_StringHash, void*, EG_BOOL);
typedef void 	(*EG_Callback_Destroy)(struct tagEG_Widget*);

/* User callbacks:
 */
typedef void 	(*EG_Callback_User_OnChange)(struct tagEG_Widget*, void*);
typedef void 	(*EG_Callback_User_OnClick)(struct tagEG_Widget*, void*);
typedef void	(*EG_Callback_User_OnEvent)(struct tagEG_Widget*, SDL_Event*, void*);
typedef void	(*EG_Callback_User_OnGotFocus)(struct tagEG_Widget*, void*);
typedef void	(*EG_Callback_User_OnLostFocus)(struct tagEG_Widget*, void*);


typedef struct tagEG_Widget
{
	/* Widget ID:
	 */
	EG_StringHash type;

	char name[EG_WIDGET_MAX_NAME_SIZE+1];
	unsigned long ID;

	/* Widget payload (widget specific data):
	 */
	void *payload_ptr;

	/* Parent window:
	 */
	void *window_ptr;

	/* A value the parent Window may ask the Widget to
	 * hold for it, never reference or change this within
	 * the Widget code, leave it to the Window to access.
	 *
	 * (It's here so I don't have to implement fast hash
	 * tables).
	 */
	long hold_value;

	/* Widget callbacks (for polymorphic interface with window):
	 */
	EG_Callback_Paint       func_paint_ptr;
	EG_Callback_SDL_Event   func_sdl_event_ptr;
	EG_Callback_Visible     func_visible_ptr;
	EG_Callback_Enabled     func_enabled_ptr;
	EG_Callback_GotFocus    func_got_focus_ptr;
	EG_Callback_LostFocus   func_lost_focus_ptr;
	EG_Callback_Stopped     func_stopped_ptr;
	EG_Callback_Attach      func_attach_ptr;
	EG_Callback_Destroy     func_destroy_ptr;

	/* User callbacks:
	 */
	EG_Callback_User_OnChange    func_user_change_ptr;
	EG_Callback_User_OnClick     func_user_click_ptr;
	EG_Callback_User_OnEvent     func_user_event_ptr;
	EG_Callback_User_OnGotFocus  func_user_gotfocus_ptr;
	EG_Callback_User_OnLostFocus func_user_lostfocus_ptr;

	void *user_change_value_ptr;
	void *user_click_value_ptr;
	void *user_event_value_ptr;
	void *user_gotfocus_value_ptr;
	void *user_lostfocus_value_ptr;

	/* Widget location:
	 */
	SDL_Rect dimension;

	/* Widget value:
	 *
	 * Numeric widgets will set numeric_value, string widgets will set
	 * string_value.  String widgets can only contain strings upto
	 * EG_WIDGET_MAX_STRING_VALUE_SIZE in length.  But it's easy to increase
	 * this above.
	 */
	double numeric_value;
	char string_value[EG_WIDGET_MAX_STRING_VALUE_SIZE+1];

	/* Widget features:
	 */

	/* stopped:
	 *
	 * Override visible, enabled and can_get_focus, effectively disables
	 * the widget.
	 * Has the same effect as:
	 * visible=EG_FALSE,
	 * enabled=EG_FALSE,
	 * can_get_focus=EG_FALSE
	 *
	 *
	 * visible:
	 *
	 * Determine if the widget paint functions will be honored, the widget
	 * will still receive events if not visible.
	 *
	 *
	 * enabeld:
	 *
	 * Determine how the widget paint functions will render the widget,
	 * no events are honored, the widget is effectively in a none usable,
	 * but visible state.
	 *
	 *
	 * can_get_focus:
	 *
	 * Determine whether the widget can receive focus or not.
	 *
	 * has_focus needs to be set by EG_Window.
	 */

	EG_BOOL visible;
	EG_BOOL enabled;
	EG_BOOL can_get_focus;
	EG_BOOL stopped;

	/* Widgets background color (foreground colors are not supported).
	 */
	SDL_Color background_color;
} EG_Widget;

#define EG_AsWidget(n) ((EG_Widget*) n)

/* User callbacks:
 */
#ifdef __cplusplus
extern "C"{
#endif

	void EG_Widget_CallDestroy(EG_Widget *widget_ptr);

#ifdef __cplusplus
}
#endif

/* Public functions:
 */

#ifdef __cplusplus
extern "C"{
#endif

	EG_BOOL EG_Widget_SetUserCallback_OnChange(EG_Widget *widget_ptr, EG_Callback_User_OnChange user_callback_ptr, void* user_change_value_ptr);
	EG_BOOL EG_Widget_SetUserCallback_OnClick(EG_Widget *widget_ptr, EG_Callback_User_OnClick user_callback_ptr, void* user_click_value_ptr);
	EG_BOOL EG_Widget_SetUserCallback_OnEvent(EG_Widget *widget_ptr, EG_Callback_User_OnEvent user_callback_ptr, void* user_event_value_ptr);
	EG_BOOL	EG_Widget_SetUserCallback_OnLostFocus(EG_Widget *widget_ptr, EG_Callback_User_OnLostFocus user_callback_ptr, void* user_lostfocus_value_ptr);
	EG_BOOL	EG_Widget_SetUserCallback_OnGotFocus(EG_Widget *widget_ptr, EG_Callback_User_OnGotFocus user_callback_ptr, void* user_gotfocus_value_ptr);

	void EG_Widget_ClearUserCallback_OnChange(EG_Widget *widget_ptr);
	void EG_Widget_ClearUserCallback_OnClick(EG_Widget *widget_ptr);
	void EG_Widget_ClearUserCallback_OnEvent(EG_Widget *widget_ptr);
	void EG_Widget_ClearUserCallback_OnLostFocus(EG_Widget *widget_ptr);
	void EG_Widget_ClearUserCallback_OnGotFocus(EG_Widget *widget_ptr);

	EG_StringHash EG_Widget_GetType(EG_Widget *widget_ptr);
	unsigned long EG_Widget_GetID(EG_Widget *widget_ptr);
	const char* EG_Widget_GetName(EG_Widget *widget_ptr);
	double 	EG_Widget_GetValue_Numeric(EG_Widget *widget_ptr);
	const char* EG_Widget_GetValue_String(EG_Widget *widget_ptr);
	SDL_Rect EG_Widget_GetDimension(EG_Widget *widget_ptr);
	SDL_Color EG_Widget_GetBackgroundColor(EG_Widget *widget_ptr);

	EG_BOOL EG_Widget_SetBackgroundColor(EG_Widget *widget_ptr, SDL_Color color);
	EG_BOOL EG_Widget_SetValue_Numeric(EG_Widget *widget_ptr, double value);
	EG_BOOL EG_Widget_SetValue_String(EG_Widget *widget_ptr, const char *string_ptr);

	// SDL_Rect EG_Widget_GetDrawingArea(EG_Widget *widget_ptr);

	EG_BOOL EG_Widget_IsFocusAllowed(EG_Widget *widget_ptr);
	EG_BOOL EG_Widget_IsVisible(EG_Widget *widget_ptr);
	EG_BOOL EG_Widget_IsEnabled(EG_Widget *widget_ptr);

	EG_BOOL EG_Widget_Repaint(EG_Widget *widget_ptr, SDL_Rect area);
	EG_BOOL EG_Widget_RepaintLot(EG_Widget *widget_ptr);

#ifdef __cplusplus
}
#endif


/* END OF widget.h -------------------------------------------------------------
 */
#endif

