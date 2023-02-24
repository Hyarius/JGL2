#pragma once

#include "jgl2_includes.h"

#include "structure/jgl2_thread.h"
#include "structure/jgl2_singleton.h"
#include "structure/Application/Core/jgl2_job_module.h"
#include "structure/Application/Core/jgl2_widget_module.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
		{
			class Job;
			class Window;
		}
	}

	namespace Abstract
	{
		namespace Application
		{
			class Core : public Singleton<Core>
			{
			public:
				using Job = std::function<int()>;

				friend class Singleton<Core>;
				friend class jgl::Widget::Module::Parenting;
				friend class jgl::Application::Module::Job;
				friend class jgl::Application::Module::Window;

			private:
				static Core* instanciate() = delete;

			private:
				bool _running = false;

				int _errorValue = 0;

				unsigned long _time;

				jgl::Application::Module::Job _jobs;
				jgl::Application::Module::Widget _widgets;

				void _updateTime();

				virtual void _setupJobs() = 0;

			protected:
				Core();

				void _addJob(Job p_job);
				void _addJob(std::string p_threadName, Job p_job);

				void _updateWidget();
				void _renderWidget();

			public:
				int run();

				template<typename TWidgetType, typename... Args>
				TWidgetType* addRootWidget(std::string p_name, Args&&... p_args) { return (_widgets.addRootWidget<TWidgetType, Args...>(p_name, std::forward<Args>(p_args)...)); }

				template<typename TWidgetType>
				TWidgetType* getRootWidget(std::string p_name) { return (_widgets.getRootWidget<TWidgetType>(p_name)); }

				template<typename TWidgetType>
				std::vector<TWidgetType*> getRootWidgetList() { return (_widgets.getRootWidgetList<TWidgetType>()); }

				std::vector<jgl::Abstract::Widget::Core*>& getAllRootWidgetsList() { return (_widgets.getAllRootWidgets()); }

				void quit(int p_errorValue = 0);

				const unsigned long& time() const { return (_time); }
			};
		}
	}
}