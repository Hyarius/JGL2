#pragma once

#include "jgl2_includes.h"

#include "structure/jgl2_thread.h"
#include "structure/jgl2_singleton.h"
#include "structure/Application/Core/jgl2_job_manager.h"
#include "structure/Application/Core/jgl2_widget_manager.h"

namespace jgl
{
	class ApplicationCore : public jgl::Singleton<ApplicationCore>
	{
	public:
		using Job = std::function<int()>;

		friend class jgl::Singleton<ApplicationCore>;
		friend class WidgetParentingManager;
		friend class JobManager;
		friend class WindowManager;

	private:
		static ApplicationCore* instanciate() = delete;

	private:
		bool _running = false;

		int _errorValue = 0;

		unsigned long _time;

		JobManager _jobManager;
		WidgetManager _widgetManager;
		
		void _updateTime();

		virtual void _setupJobs() = 0;

	protected:
		ApplicationCore();

		void _addJob(Job p_job);
		void _addJob(std::string p_threadName, Job p_job);

		void _updateWidget();
		void _renderWidget();

	public:
		int run();

		template<typename TWidgetType, typename... Args>
		TWidgetType* addRootWidget(std::string p_name, Args&&... p_args){ return (_widgetManager.addRootWidget<TWidgetType, Args...>(p_name, std::forward<Args>(p_args)...)); }

		template<typename TWidgetType>
		TWidgetType* getRootWidget(std::string p_name) { return (_widgetManager.getRootWidget<TWidgetType>(p_name)); }

		template<typename TWidgetType>
		std::vector<TWidgetType*> getRootWidgetList(){ return (_widgetManager.getRootWidgetList<TWidgetType>()); }

		std::vector<WidgetCore*>& getAllRootWidgetsList() { return (_widgetManager.getAllRootWidgets()); }

		void quit(int p_errorValue = 0);

		const unsigned long& time() const {return (_time);}
	};
}