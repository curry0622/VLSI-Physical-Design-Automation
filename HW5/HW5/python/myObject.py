class Die:
    def __init__(self, design_name, x1, y1, x2, y2):
        self.design_name = design_name
        self._x1 = x1
        self._y1 = y1
        self._x2 = x2
        self._y2 = y2

    def setPos(self, x1, y1, x2, y2):
        self._x1 = x1
        self._y1 = y1
        self._x2 = x2
        self._y2 = y2


class Component:
    def __init__(self, lib_name, inst_name, x, y):
        self.lib_name = lib_name
        self.inst_name = inst_name
        self._x = x
        self._y = y

    def setPos(self, x, y):
        self._x = x
        self._y = y


class SpecialNet:
    def __init__(self, inst_name, layer, x1, y1, x2, y2):
        self.inst_name = inst_name
        self.layer = layer
        self._x1 = x1
        self._y1 = y1
        self._x2 = x2
        self._y2 = y2

    def setPos(self, x1, y1, x2, y2):
        self._x1 = x1
        self._y1 = y1
        self._x2 = x2
        self._y2 = y2
