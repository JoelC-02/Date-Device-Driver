cmd_/home/joelc/CN/date_driver/date.mod := printf '%s\n'   date.o | awk '!x[$$0]++ { print("/home/joelc/CN/date_driver/"$$0) }' > /home/joelc/CN/date_driver/date.mod
