/* from http://stackoverflow.com/questions/6058139/
        why-is-matlab-so-fast-in-matrix-multiplication */
#include <vector>

template <typename T>
struct matrix {
    matrix(unsigned int x, unsigned int y): n_row(x), n_col(y), M(x * y) {}
    unsigned int n_row;
    unsigned int n_col;
    std::vector<T> M;
    T &operator()(unsigned int i, unsigned int j);
};

template <typename T>
T &matrix<T>::operator()(unsigned int i, unsigned int j) {
    return M[n_col * i + j];
}
