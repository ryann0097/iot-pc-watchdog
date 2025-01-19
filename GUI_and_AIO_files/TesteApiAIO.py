import WdAdaMailer

# Create an instance of AdaMailer
ada = WdAdaMailer.AdaMailer(50, 30, 70, 1)

# Send the CPU value
ada.send_cpu()
ada.send_temp()
ada.send_mem()
