import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QLineEdit, QPushButton
import subprocess
from scipy.optimize import minimize
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import time
import threading








class MainWindow(QMainWindow):

    

    def __init__(self):

        super().__init__()
        
        self.setWindowTitle("Optimization Tool : Design Tuning Fork")
        self.setGeometry(700, 500, 700, 500)
        
        
        self.label_freq = QLabel("Desired frequency : ", self)
        self.label_freq.move(10, 80) #170
        self.label_freq.setFixedWidth(200)

        self.lineedit_freq = QLineEdit(self)
        self.lineedit_freq.move(180, 80)


        self.label_prec = QLabel("Desired precision : \n\n* >= 0.5 for speed \n* < 0.3 for precision", self)
        self.label_prec.move(10, 160) #170
        self.label_prec.setFixedWidth(300)
        self.label_prec.setFixedHeight(100)

        self.lineedit_prec = QLineEdit(self)
        self.lineedit_prec.move(180,170)

        
        self.button = QPushButton("Run Optimization", self)
        self.button.move(400, 125) #200
        self.button.setFixedWidth(200)
        self.button.clicked.connect(self.sub_process)
        

        self.label_INIT = QLabel("LINMA1170.\nThis tool provides an interactive way to get optimal parameters of a tuning fork.",self)
        self.label_INIT.move(10,10)
        self.label_INIT.setFixedWidth(700)
        self.label_INIT.setFixedHeight(50)


        self.label_C = QLabel("",self)
        self.label_C.move(10,260)

        self.label_result = QLabel("", self)
        self.label_result.move(200, 300) #240
        self.label_result.setFixedWidth(600)

        self.label_optiarg = QLabel("", self)
        self.label_optiarg.move(150, 370) #240
        self.label_optiarg.setFixedWidth(600)

        self.label_time = QLabel("",self)
        self.label_time.setFixedWidth(600)
        self.label_time.move(200, 400)


    def run_optimization(self):

        self.label_optiarg.setText("")
        self.label_time.setText("")

        
        desiredFrequency = float(self.lineedit_freq.text())
        desiredPrecision = float(self.lineedit_prec.text())

        k = 2
        out = "out_test.txt"
        r1 = 6e-3 
        r2 = 11e-3
        e = 38e-3 
        l = 82e-3
        args = [r1, r2, e, l]


        def getFrequency(arguments, meshSize):
            r1 = arguments[0]
            r2 = arguments[1]
            e =  arguments[2]
            l = arguments[3]
            QApplication.processEvents()
            subprocess.check_call([r"./project", str(k), out, str(r1), str(r2), str(e), str(l), str(meshSize)])
            QApplication.processEvents()

            with open(out) as f :
                eigenvalues = f.read().split()
            
            self.label_result.setText("Current frequency : {:.2f}".format(float(eigenvalues[1])))
            
            return float(eigenvalues[1])

        def objective_function(arguments, desiredFrequency, meshSize):
            return (getFrequency(arguments, meshSize) - desiredFrequency)**2

        
        initial_guess = args

        tic = time.time()
        result = minimize(objective_function, initial_guess, args=(desiredFrequency, desiredPrecision), tol=1, method='Nelder-Mead')
        toc = time.time()

        optimal_arguments = result.x

        self.label_optiarg.setText("Optimal arguments : {}".format(optimal_arguments))
        self.label_time.setText("       Runtime : {:.2f} sec".format(toc-tic))

    def sub_process(self):
        self.label_C.setText("Information :")
        thread = threading.Thread(target=self.run_optimization)
        thread.start()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
