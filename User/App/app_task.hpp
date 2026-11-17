/**
 * @file app_task.hpp
 * @brief 应用层任务入口（工程默认只有系统任务，业务任务后续自行添加）
 */

#ifndef __APP_TASK_HPP__
#define __APP_TASK_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 创建系统任务并返回（在调度器启动前调用）
 */
void app_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_TASK_HPP__ */
