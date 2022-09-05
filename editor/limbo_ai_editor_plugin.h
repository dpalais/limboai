/* limbo_ai_editor_plugin.h */

#ifdef TOOLS_ENABLED
#ifndef LIMBO_AI_EDITOR_PLUGIN_H
#define LIMBO_AI_EDITOR_PLUGIN_H

#include "core/object.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "modules/limboai/bt/behavior_tree.h"
#include "modules/limboai/bt/bt_task.h"
#include "scene/gui/box_container.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/tree.h"
#include "scene/resources/texture.h"

class TaskTree : public Control {
	GDCLASS(TaskTree, Control);

private:
	Tree *tree;
	Ref<BehaviorTree> bt;
	Ref<BTTask> last_selected;
	bool editable;

	TreeItem *_create_tree(const Ref<BTTask> &p_task, TreeItem *p_parent, int p_idx = -1);
	void _update_item(TreeItem *p_item);
	void _update_tree();
	TreeItem *_find_item(const Ref<BTTask> &p_task) const;

	void _on_item_selected();
	void _on_item_rmb_selected(const Vector2 &p_pos);
	void _on_task_changed();

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

protected:
	static void _bind_methods();

public:
	void load_bt(const Ref<BehaviorTree> &p_behavior_tree);
	Ref<BehaviorTree> get_bt() const { return bt; }
	void update_tree() { _update_tree(); }
	void update_task(const Ref<BTTask> &p_task);
	Ref<BTTask> get_selected() const;
	void deselect();

	TaskTree();
	~TaskTree();
};

class TaskSection : public VBoxContainer {
	GDCLASS(TaskSection, VBoxContainer);

private:
	FlowContainer *tasks_container;
	Button *section_header;

	void _on_task_button_pressed(const StringName &p_task);
	void _on_header_pressed();

protected:
	static void _bind_methods();

public:
	void set_filter(String p_filter);
	void add_task_button(String p_name, const Ref<Texture> &icon, Variant p_meta);

	TaskSection(String p_category_name, EditorNode *p_editor);
	~TaskSection();
};

class TaskPanel : public PanelContainer {
	GDCLASS(TaskPanel, PanelContainer)

private:
	EditorNode *editor;
	LineEdit *filter_edit;
	VBoxContainer *sections;

	void _populate_core_tasks_from_class(const StringName &p_base_class, List<String> *p_task_classes);
	void _populate_scripted_tasks_from_dir(String p_path, List<String> *p_task_classes);
	void _on_task_button_pressed(const StringName &p_task);
	void _on_filter_text_changed(String p_text);

protected:
	static void _bind_methods();

public:
	void refresh();

	TaskPanel(EditorNode *p_editor);
	~TaskPanel();
};

class LimboAIEditor : public Control {
	GDCLASS(LimboAIEditor, Control);

private:
	enum Action {
		ACTION_REMOVE,
		ACTION_MOVE_UP,
		ACTION_MOVE_DOWN,
		ACTION_DUPLICATE,
		ACTION_MAKE_ROOT,
	};

	EditorNode *editor;
	Vector<Ref<BehaviorTree>> history;
	int idx_history;
	Set<Ref<BehaviorTree>> dirty;

	Button *header;
	TaskTree *task_tree;
	Panel *usage_hint;
	PopupMenu *menu;
	FileDialog *save_dialog;
	FileDialog *load_dialog;
	Button *history_back;
	Button *history_forward;
	TaskPanel *task_panel;

	void _add_task(const Ref<BTTask> &p_task);
	_FORCE_INLINE_ void _add_task_with_prototype(const Ref<BTTask> &p_prototype) { _add_task(p_prototype->clone()); }
	void _update_header();
	void _update_history_buttons();
	void _new_bt();
	void _save_bt(String p_path);
	void _load_bt(String p_path);
	void _mark_as_dirty(bool p_dirty);

	void _on_tree_rmb(const Vector2 &p_menu_pos);
	void _on_action_selected(int p_id);
	void _on_tree_task_selected(const Ref<BTTask> &p_task) const;
	void _on_visibility_changed() const;
	void _on_header_pressed() const;
	void _on_save_pressed();
	void _on_panel_task_selected(String p_task);
	void _on_history_back();
	void _on_history_forward();
	void _on_task_dragged(Ref<BTTask> p_task, Ref<BTTask> p_to_task, int p_type);

protected:
	static void _bind_methods();

public:
	static Ref<Texture> get_task_icon(String p_script_path_or_class);

	void edit_bt(Ref<BehaviorTree> p_behavior_tree);

	void apply_changes();

	LimboAIEditor(EditorNode *p_editor);
	~LimboAIEditor();
};

class LimboAIEditorPlugin : public EditorPlugin {
	GDCLASS(LimboAIEditorPlugin, EditorPlugin);

private:
	EditorNode *editor;
	LimboAIEditor *limbo_ai_editor;

protected:
	void _notification(int p_notification);

public:
	virtual String get_name() const { return "LimboAI"; }
	virtual const Ref<Texture> get_icon() const;
	bool has_main_screen() const { return true; }
	virtual void make_visible(bool p_visible);
	virtual void apply_changes();
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;

	LimboAIEditorPlugin(EditorNode *p_editor);
	~LimboAIEditorPlugin();
};

#endif // TOOLS_ENABLED

#endif // LIMBO_AI_EDITOR_PLUGIN_H