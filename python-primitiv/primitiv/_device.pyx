from libcpp.vector cimport vector

from primitiv._shape cimport normShape
from primitiv._tensor cimport wrapTensor, _Tensor


cdef class _Device:

    @staticmethod
    def get_default():
        return wrapDevice(&CppDevice_get_default())

    @staticmethod
    def set_default(_Device dev):
        CppDevice_set_default(dev.wrapped[0])

    def dump_description(self):
        self.wrapped.dump_description()
        return

    def __copy__(self):
        raise NotImplementedError(type(self).__name__ + " does not support `__copy__` for now.")

    def __deepcopy__(self, memo):
        raise NotImplementedError(type(self).__name__ + " does not support `__deepcopy__` for now.")