import sys

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QApplication, QMainWindow)
from rendererGL import RendererGL
from array import array
from cpp.Polygonizer import polygonize, n_funcs, func_name
import numpy as np


class IS_widget(RendererGL):
    coords = None
    normals = None
    color = 0.4, 1, 1 # main color
    win = None

    def __init__(self, mesh, win):
        super(IS_widget, self).__init__()
        self.win = win
        self.get_mesh(mesh)
        self.setFocusPolicy(Qt.StrongFocus)  # accepts key events

    def get_mesh(self, mesh):
        self.coords = mesh[0]  # n coords x 3
        self.normals = mesh[1]
        self.indices = mesh[2]

    def init(self, gl):
        def set_draw(gl):
            def np2array(type, np_vect):  # array = numpy, take the fast lane
                arr = array(type)
                arr.frombytes(np_vect.tobytes())
                return arr

            gl.glEnableClientState(gl.GL_VERTEX_ARRAY)  # define draw components
            gl.glEnableClientState(gl.GL_NORMAL_ARRAY)

            gl.glVertexPointer(3, gl.GL_DOUBLE, 0, np2array('d', self.coords))  # set pointers
            gl.glNormalPointer(gl.GL_DOUBLE, 0, np2array('d', self.normals))

            self.index_array = np2array('I', self.indices)  # index array & n items
            self.n_index = self.indices.shape[0]  # n items of array

        self.sceneInit(gl)
        gl.glCullFace(gl.GL_FRONT)

        set_draw(gl)

    def draw(self, gl):
        def draw_elements(gl):
            gl.glEnable(gl.GL_NORMALIZE)
            gl.glColor3fv(self.color) # main mesh color
            gl.glDrawElements(gl.GL_TRIANGLES, self.n_index, gl.GL_UNSIGNED_INT, self.index_array)

        sc = 0.3
        gl.glScalef(sc, sc, sc)

        draw_elements(gl)


class Main(QMainWindow):
    def __init__(self, n_func, mesh, *args):
        super(Main, self).__init__(*args)

        self.setWindowTitle(f'Implicit surfaces: {func_name(n_func)}')
        self.setCentralWidget(IS_widget(mesh, self))
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)

    n_func, nDiv = 12, 100 # nDiv is the mesh resolution n x n x n
    print(f'# predef funcs: {n_funcs()}, selected #: {n_func}, name: {func_name(n_func)}')

    Main(n_func, polygonize(nDiv=nDiv, nFunc=n_func))

    app.exec_()
