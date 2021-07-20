/**
 * This file is a part of channelflow version 2.0 https://channelflow.ch.
 * License is GNU GPL version 2 or later: ./LICENCE
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/complex.h>
#include "channelflow/diffops.h"
#include "channelflow/flowfield.h"
#include "channelflow/symmetry.h"

using namespace std;
namespace py = pybind11;

namespace chflow {

/*** Access functions for FlowField values ***/
// Real FlowField_get_val(FlowField& self, py::tuple t) {
//     // A check that len(t) = 4 would be helpful
//     if (self.xzstate() == Spectral)
//         throw runtime_error("Trying to access physical data, but FlowField is in spectral state");
//     int nx = py::cast<int>(t[0]);
//     int ny = py::cast<int>(t[1]);
//     int nz = py::cast<int>(t[2]);
//     int i = py::cast<int>(t[3]);
//     return self(nx, ny, nz, i);
// }

// void FlowField_set_val(FlowField& self, py::tuple t, Real val) {
//     if (self.xzstate() == Spectral)
//         throw runtime_error("Trying to access physical data, but FlowField is in spectral state");
//     int nx = py::cast<int>(t[0]);
//     int ny = py::cast<int>(t[1]);
//     int nz = py::cast<int>(t[2]);
//     int i = py::cast<int>(t[3]);
//     self(nx, ny, nz, i) = val;
// }

Complex FlowField_get_cmplx(FlowField& self, py::tuple t) {
    if (self.xzstate() == Physical)
        throw runtime_error("Trying to access spectral data, but FlowField is in physical state");
    int nx = py::cast<int>(t[0]);
    int ny = py::cast<int>(t[1]);
    int nz = py::cast<int>(t[2]);
    int i = py::cast<int>(t[3]);
    return self.cmplx(nx, ny, nz, i);
}

void FlowField_set_cmplx(FlowField& self, py::tuple t, Complex val) {
    if (self.xzstate() == Physical)
        throw runtime_error("Trying to access spectral data, but FlowField is in physical state");
    int nx = py::cast<int>(t[0]);
    int ny = py::cast<int>(t[1]);
    int nz = py::cast<int>(t[2]);
    int i = py::cast<int>(t[3]);
    self.cmplx(nx, ny, nz, i) = val;
}

/*** Wrapper functions ***/
// They are needed because Python can't handle overloaded functions. So for each
// function with a default argument, we need a wrapper function that has a fixed
// number of arguments
// Real L2Norm_wrapped(const FlowField& f) { return L2Norm(f); }
// Real L2Norm2_wrapped(const FlowField& f) { return L2Norm2(f); }
// Real L2Dist_wrapped(const FlowField& f, const FlowField& g) { return L2Dist(f, g); }
Real L2IP_wrapped(const FlowField& f, const FlowField& g) {
    py::gil_scoped_release release;
    Real res = L2InnerProduct(f, g);
    py::gil_scoped_acquire acquire;
    return res;
}
// Real wallshear_wrapped(const FlowField& f) { return wallshear(f); }
// Real wallshearUpper_wrapped(const FlowField& f) { return wallshearUpper(f); }
// Real wallshearLower_wrapped(const FlowField& f) { return wallshearLower(f); }
// Real L2Norm3d_wrapped(const FlowField& f) { return L2Norm3d(f); }
// Real Ecf_wrapped(const FlowField& f) { return Ecf(f); }
// FlowField curl_wrapped(const FlowField& f) { return curl(f); }
// FlowField lapl_wrapped(const FlowField& f) { return lapl(f); }
// FlowField grad_wrapped(const FlowField& f) { return grad(f); }
// FlowField div_wrapped(const FlowField& f) { return div(f); }

// FlowField diff_wrapped(const FlowField& f, int i, int n) { return diff(f, i, n); }
void FlowField_save(const FlowField& self, string filebase) { self.save(filebase); }

/*** The actual python module ***/
PYBIND11_MODULE(libpycf, m) {
    py::class_<FlowField>(m, "FlowField")
        .def(py::init<string>())
        .def(py::init<FlowField>())
        .def(py::init<int, int, int, int, Real, Real, Real, Real>())
        .def("save", &FlowField_save)
        // .def("get", &FlowField_get_val)
        // .def("cmplx", &FlowField_get_cmplx)
        .def("__getitem__", &FlowField_get_cmplx)
        .def("__setitem__", &FlowField_set_cmplx)
        // .def("makePhysical", &FlowField::makePhysical)
        // .def("makeSpectral", &FlowField::makeSpectral)
        // .def("setToZero", &FlowField::setToZero)
        .def_property_readonly("Nx", &FlowField::Nx)
        .def_property_readonly("Ny", &FlowField::Ny)
        .def_property_readonly("Nz", &FlowField::Nz)
        // .def_property_readonly("Nd", &FlowField::Nd)
        // .def_property_readonly("Mx", &FlowField::Mx)
        // .def_property_readonly("My", &FlowField::My)
        // .def_property_readonly("Mz", &FlowField::Mz)
        .def_property_readonly("Lx", &FlowField::Lx)
        .def_property_readonly("Ly", &FlowField::Ly)
        // .def_property_readonly("Lz", &FlowField::Lz)
        .def_property_readonly("a", &FlowField::a)
        .def_property_readonly("b", &FlowField::b)
        // .def("x", &FlowField::x)
        // .def("y", &FlowField::y)
        // .def("z", &FlowField::z)
        .def(py::self *= Real())
        .def(Real() * py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self);

    // py::class_<FieldSymmetry>(m, "FieldSymmetry")
    //     .def(py::init<FieldSymmetry>())
    //     .def(py::init<string>())
    //     .def(py::init<int, int, int, Real, Real, int>())
    //     .def_property_readonly("sx", &FieldSymmetry::sx)
    //     .def_property_readonly("sy", &FieldSymmetry::sy)
    //     .def_property_readonly("sz", &FieldSymmetry::sz)
    //     .def_property_readonly("ax", &FieldSymmetry::ax)
    //     .def_property_readonly("az", &FieldSymmetry::az)
    //     .def("__call__",
    //          static_cast<FlowField (FieldSymmetry::*)(const FlowField& u) const>(&FieldSymmetry::operator()));

    // m.def("L2Norm", &L2Norm_wrapped);
    // m.def("L2Norm2", &L2Norm2_wrapped);
    // m.def("L2Dist", &L2Dist_wrapped);
    m.def("L2IP", &L2IP_wrapped);
    // m.def("wallshear", &wallshear_wrapped);
    // m.def("wallshearLower", &wallshearLower_wrapped);
    // m.def("wallshearUpper", &wallshearUpper_wrapped);
    // m.def("L2Norm3d", &L2Norm3d_wrapped);
    // m.def("curl", &curl_wrapped);
}

}  // namespace chflow
