//
// Created by 24547 on 2026/5/30.
//

#ifndef MULTIFUNCTIONAL_MONITOR_STATION_MODE_H
#define MULTIFUNCTIONAL_MONITOR_STATION_MODE_H

typedef enum {
    Mode_realtime_display,
    Mode_history_display,
    Mode_linechat
} Mode_name;

extern Mode_name mode;

typedef void (*Mode_handler)(void);

typedef struct {
    Mode_name mode;
    Mode_handler handler;
} Mode;

void Mode_realtime_disp(void);
void Mode_line_chart(void);
void Mode_Process(void);

#endif //MULTIFUNCTIONAL_MONITOR_STATION_MODE_H
