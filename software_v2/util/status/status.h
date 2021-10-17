#ifndef UTIL_STATUS_H_
#define UTIL_STATUS_H_

#define ERROR_PIN       (2U)
#define LOADING_PIN     (1U)
#define OK_PIN          (0U)

void status_error(bool status);
void status_loading(bool status);
void status_ok(bool status);
void status_loading_flash(void);

#endif