// -*- C++ -*-                                                                 |
//-----------------------------------------------------------------------------+
//                                                                             |
// inln.h                                                                      |
//                                                                             |
// C++ code                                                                    |
//                                                                             |
// Copyright Walter Dehnen, 1994-2003                                          |
// e-mail:   wdehnen@aip.de                                                    |
// address:  Astrophysikalisches Institut Potsdam,                             |
//           An der Sternwarte 16, D-14482 Potsdam, Germany                    |
//                                                                             |
//-----------------------------------------------------------------------------+
#ifndef falcON_included_inln_h
#define falcON_included_inln_h

#ifndef falcON_included_cstdlib
#  include <cstdlib>
#  define falcON_included_cstdlib
#endif
#ifndef falcON_included_iostream
#  include <iostream>
#  define falcON_included_iostream
#endif
#ifndef falcON_included_cmath
#  include <cmath>
#  define falcON_included_cmath
#endif
#ifndef falcON_included_exit_h
#  include <public/exit.h>
#endif
//------------------------------------------------------------------------------
namespace nbdy {
  using std::abs;
#if(0) // general abs is not defined in the C++ std headers
  template<typename scalar_type> inline
  const scalar_type   abs     (const scalar_type&x)
  { return (x<0)? -x : x; }
  //----------------------------------------------------------------------------
  template<> inline
  const double abs<double> (const double&x)
  { return fabs(x); }
  //----------------------------------------------------------------------------
  template<> inline
  const float abs<float>   (const float &x) 
  { return fabs(x); }
#endif
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const int sign    (const scalar_type&x)
  { return (x<0)? -1:((x>0)? 1:0 ); }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   sign    (const scalar_type&x,
			       const scalar_type&s)
  { return ( s>0 )?  abs(x) : -abs(x); }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type&  min     (const scalar_type&x,
			       const scalar_type&y)
  { return (x<y)? x : y; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   min     (const scalar_type&x,
			       const scalar_type&y,
			       const scalar_type&z)
  { return min(x,min(y,z)); }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type&  max     (const scalar_type&x,
			       const scalar_type&y)
  { return (x>y)? x : y; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   max     (const scalar_type&x,
			       const scalar_type&y,
			       const scalar_type&z)
  { return max(x,max(y,z)); }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   mod     (const scalar_type&x,
			       const scalar_type&y)
  { return x-y*int(x/y); }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   twice   (const scalar_type&x)
  { return x+x; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   trice   (const scalar_type&x)
  { return 3*x; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   times4  (const scalar_type&x)
  { return 4*x; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   square  (const scalar_type&x)
  { return x*x; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  const scalar_type   cube    (const scalar_type&x)
  { return x*x*x; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  void      swap    (scalar_type&a, scalar_type&b)
  { register scalar_type t=a; a=b; b=t; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  scalar_type sqrt0(scalar_type const&x)
  { return x <= scalar_type(0)? scalar_type(0) : std::sqrt(x); }
  //----------------------------------------------------------------------------
#if (0) // integer power of floating point number is now supported by cmath
  template<typename _Tp> inline
  _Tp pow(const _Tp &x, unsigned int n)
  {
    if(n==0) return _Tp(1);
    register _Tp z=x, y=(n%2)? x : _Tp(1);
    for(register unsigned int i=n>>1; i; i>>=1) { z*=z; if(i%2) y*=z; }
    return y;
  }
#endif
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  void update_max(scalar_type&x, const scalar_type&y)
  { if(y>x) x=y; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  void update_min(scalar_type&x, const scalar_type&y)
  { if(y<x) x=y; }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  char* negspace(const scalar_type&x)
  { return (x<scalar_type(0))? " " : "  "; }
  //----------------------------------------------------------------------------
  inline bool is_integral(const float&x)
  { return floor(abs(x))==abs(x);  }
  //----------------------------------------------------------------------------
  inline bool is_integral(const double&x)
  { return floor(abs(x))==abs(x);  }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  std::ostream& write_array(std::ostream&s, const scalar_type* x, unsigned N)
  {
    s << x[0];
    for(register unsigned i=1; i!=N; ++i) s<<" "<<x[i];
    return s;
  }
  //----------------------------------------------------------------------------
  template<typename scalar_type> inline
  std::istream& read_array(std::istream&s, scalar_type* x, unsigned N) {
    register scalar_type y[N];
    char c=0;
    s >> c;
    if(c == '(') {
      for(register unsigned i=0; i!=N; ++i) s >> y[i];
      s >> c;
      if(c != ')') s.clear(std::ios::badbit);
    } else {
      s.putback(c);
      for(register unsigned i=0; i!=N; ++i) s >> y[i];
    }
    for(register unsigned i=0; i!=N; ++i) x[i] = y[i];
    return s;
  }
}
//------------------------------------------------------------------------------
#endif // falcON_included_inln_h
