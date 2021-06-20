Matrix(const Matrix *parent, bool h_split, bool first)
{ // constructor for views on matrices
  T *firstsubrow = nullptr;
  origcols = parent->origcols; /* inherit from parent */
  if (h_split) {
    if (first) /* h_split_upper code */
    else       /* h_split_lower code */
  } else { /* !h_split */
    if (first) /* v_split_left code */
    else       /* v_split_right code */
  }
  hasownvalues = false; // it is a view => no own values
  initialized = parent->initialized;
  M = new T *[n_rows];  // has its own row pointers
  M[0] = firstsubrow;   // was determined in all cases
  for (size_t r = 1; r < n_rows; r++) 
    M[r] = M[r-1] + parent->origcols; // set subrow pointers
}
