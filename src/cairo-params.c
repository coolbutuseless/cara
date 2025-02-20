
#define R_NO_REMAP

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include "include/cairo.h"

#include "nr-utils.h"
#include "color.h"
#include "cairo-core.h"
#include "cairo-params.h"




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Parse a R filter type to a cairo filter type
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cairo_filter_t parse_filter(SEXP filter_) {
  
  if (Rf_isNull(filter_)) {
    return CAIRO_FILTER_GOOD;
  }
  
  const char *filter = CHAR(STRING_ELT(filter_, 0));
  cairo_filter_t filter_type = CAIRO_FILTER_GOOD;
  
  if (strcmp(filter, "fast") == 0) {
    filter_type = CAIRO_FILTER_FAST;  
  } else if (strcmp(filter, "good") == 0) {
    filter_type = CAIRO_FILTER_GOOD;  
  } else if (strcmp(filter, "best") == 0) {
    filter_type = CAIRO_FILTER_BEST;  
  } else if (strcmp(filter, "nearest") == 0) {
    filter_type = CAIRO_FILTER_NEAREST;  
  } else if (strcmp(filter, "bilinear") == 0) {
    filter_type = CAIRO_FILTER_BILINEAR;  
  } else {
    Rf_error("Filter type unknown: '%s'", filter);
  }
  
  return filter_type;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Antialias
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cr_params_set_antialias(cairo_t *cr, SEXP type_) {
  
  if (TYPEOF(type_) != STRSXP) {
    Rf_error("cr_params_set_antialias(): must be a string");
  }
  if (Rf_length(type_) != 1) {
    Rf_error("cr_params_set_antialias(): must be a single string. Got %i", Rf_length(type_));
  }
  
  const char *type_str = CHAR(STRING_ELT(type_, 0));
  
  cairo_antialias_t type = CAIRO_ANTIALIAS_DEFAULT;
  if (strcmp(type_str, "none") == 0) {
    type = CAIRO_ANTIALIAS_NONE;
  } else if (strcmp(type_str, "gray") == 0) {
    type = CAIRO_ANTIALIAS_GRAY;
  } else if (strcmp(type_str, "default") == 0) {
    type = CAIRO_ANTIALIAS_DEFAULT;
  } else if (strcmp(type_str, "subpixel") == 0) {
    type = CAIRO_ANTIALIAS_SUBPIXEL;
  } else if (strcmp(type_str, "fast") == 0) {
    type = CAIRO_ANTIALIAS_FAST;
  } else if (strcmp(type_str, "good") == 0) {
    type = CAIRO_ANTIALIAS_GOOD;
  } else if (strcmp(type_str, "best") == 0) {
    type = CAIRO_ANTIALIAS_BEST;
  } else {
    Rf_error("cr_params_set_antialias(): Unknown type: '%s'\n", type_str);
  } 
  
  cairo_set_antialias(cr, type);
  return;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cr_params_set_operator(cairo_t *cr, SEXP type_) {
  if (TYPEOF(type_) != STRSXP) {
    Rf_error("cr_params_set_operator(): must be a string");
  }
  if (Rf_length(type_) != 1) {
    Rf_error("cr_params_set_operator(): must be a single string. Got %i", Rf_length(type_));
  }
  
  const char *type_str = CHAR(STRING_ELT(type_, 0));
  cairo_operator_t type = CAIRO_OPERATOR_OVER;
  
  if (strcmp(type_str, "clear") == 0) {
    type = CAIRO_OPERATOR_CLEAR;
  } else if (strcmp(type_str, "source") == 0) {
    type = CAIRO_OPERATOR_SOURCE;
  } else if (strcmp(type_str, "over") == 0) {
    type = CAIRO_OPERATOR_OVER;
  } else if (strcmp(type_str, "in") == 0) {
    type = CAIRO_OPERATOR_IN;
  } else if (strcmp(type_str, "out") == 0) {
    type = CAIRO_OPERATOR_OUT;
  } else if (strcmp(type_str, "atop") == 0) {
    type = CAIRO_OPERATOR_ATOP;
  } else if (strcmp(type_str, "dest") == 0) {
    type = CAIRO_OPERATOR_DEST;
  } else if (strcmp(type_str, "dest-over") == 0) {
    type = CAIRO_OPERATOR_DEST_OVER;
  } else if (strcmp(type_str, "dest-in") == 0) {
    type = CAIRO_OPERATOR_DEST_IN;
  } else if (strcmp(type_str, "dest-out") == 0) {
    type = CAIRO_OPERATOR_DEST_OUT;
  } else if (strcmp(type_str, "dest-atop") == 0) {
    type = CAIRO_OPERATOR_DEST_ATOP;
  } else if (strcmp(type_str, "xor") == 0) {
    type = CAIRO_OPERATOR_XOR;
  } else if (strcmp(type_str, "add") == 0) {
    type = CAIRO_OPERATOR_ADD;
  } else if (strcmp(type_str, "saturate") == 0) {
    type = CAIRO_OPERATOR_SATURATE;
  } else if (strcmp(type_str, "multiply") == 0) {
    type = CAIRO_OPERATOR_MULTIPLY;
  } else if (strcmp(type_str, "screen") == 0) {
    type = CAIRO_OPERATOR_SCREEN;
  } else if (strcmp(type_str, "overlay") == 0) {
    type = CAIRO_OPERATOR_OVERLAY;
  } else if (strcmp(type_str, "darken") == 0) {
    type = CAIRO_OPERATOR_DARKEN;
  } else if (strcmp(type_str, "lighten") == 0) {
    type = CAIRO_OPERATOR_LIGHTEN;
  } else if (strcmp(type_str, "dodge") == 0) {
    type = CAIRO_OPERATOR_COLOR_DODGE;
  } else if (strcmp(type_str, "burn") == 0) {
    type = CAIRO_OPERATOR_COLOR_BURN;
  } else if (strcmp(type_str, "hard-light") == 0) {
    type = CAIRO_OPERATOR_HARD_LIGHT;
  } else if (strcmp(type_str, "soft-light") == 0) {
    type = CAIRO_OPERATOR_SOFT_LIGHT;
  } else if (strcmp(type_str, "difference") == 0) {
    type = CAIRO_OPERATOR_DIFFERENCE;
  } else if (strcmp(type_str, "exclusion") == 0) {
    type = CAIRO_OPERATOR_EXCLUSION;
  } else if (strcmp(type_str, "hsl-hue") == 0) {
    type = CAIRO_OPERATOR_HSL_HUE;
  } else if (strcmp(type_str, "hsl-sat") == 0) {
    type = CAIRO_OPERATOR_HSL_SATURATION;
  } else if (strcmp(type_str, "hsl-color") == 0) {
    type = CAIRO_OPERATOR_HSL_COLOR;
  } else if (strcmp(type_str, "hsl-luminosity") == 0) {
    type = CAIRO_OPERATOR_HSL_LUMINOSITY;
  } else {
    Rf_error("cr_params_set_operator(): Unknown type: '%s'\n", type_str);
  } 
  
  cairo_set_operator(cr, type);
  return;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Line Dash
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cr_params_set_line_dash(cairo_t *cr, SEXP dash_) {
  
  int num_dashes = Rf_length(dash_);
  if (num_dashes == 0) {
    Rf_error("'line-dash': number of dashes must be > 0");
  }
  if (!Rf_isReal(dash_)) {
    Rf_error("'line-dash': must be a numeric vector");
  }
  
  double offset = 0;
  cairo_set_dash(cr, REAL(dash_), num_dashes, offset);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Apply the parameters 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cr_params_set(cairo_t *cr, SEXP params_) {
  
  if (Rf_isNull(params_)) {
    return;
  }
  
  if (!Rf_isNewList(params_)) {
    Rf_error("apply_params(): 'params' must be a list");
  }
  
  int N = Rf_length(params_);
  if (N == 0) {
    return;
  }
  
  SEXP nms_ = Rf_getAttrib(params_, R_NamesSymbol);
  if (Rf_isNull(nms_) || Rf_length(nms_) == 0) {
    Rf_error("apply_params(): 'params' must be a named list");
  }
  
  int nprotect = 0;
  
  for (int i = 0; i < N; i++) {
    const char *nm = CHAR(STRING_ELT(nms_, i));
    SEXP value_ = VECTOR_ELT(params_, i);
    if (strcmp(nm, "antialias") == 0) {
      cr_params_set_antialias(cr, value_);
    } else if (strcmp(nm, "line_width") == 0) {
      if (!Rf_isNumeric(value_)) {
        Rf_error("'line_width' must be numeric");
      }
      cairo_set_line_width(cr, Rf_asReal(value_));
    } else if (strcmp(nm, "line_end") == 0) {
      Rprintf("set_params(): line_end\n");
      if (TYPEOF(value_) != STRSXP) {
        Rf_error("'line_end' must a string");
      }
      const char *line_end = CHAR(STRING_ELT(value_, 0));
      if (strcmp(line_end, "round") == 0) {
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
      } else if (strcmp(line_end, "butt") == 0) {
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
      } else if (strcmp(line_end, "square") == 0) {
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
      } else {
        Rf_error("'line_end' not understood: '%s'", line_end);
      }
    } else if (strcmp(nm, "line_join") == 0) {
      const char *line_join = CHAR(STRING_ELT(value_, 0));
      if (strcmp(line_join, "miter") == 0 || strcmp(line_join, "mitre") == 0) {
        cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
      } else if (strcmp(line_join, "round") == 0) {
        cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
      } else if (strcmp(line_join, "bevel") == 0) {
        cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);
      } else {
        Rf_error("Unknown line-join type '%s'", line_join);
      }
    } else if (strcmp(nm, "line_mitre_limit") == 0) {
      double mitre_limit = Rf_asReal(value_);
      cairo_set_miter_limit(cr, mitre_limit);
    } else if (strcmp(nm, "line_dash") == 0) {
      cr_params_set_line_dash(cr, value_);
    } else if (strcmp(nm, "operator") == 0) {
      cr_params_set_operator(cr, value_);
    } else if (strcmp(nm, "fill_rule") == 0) {
      const char *fill_rule = CHAR(STRING_ELT(value_, 0));
      if (strcmp(fill_rule, "winding") == 0) {
        cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
      } else if (strcmp(fill_rule, "even-odd") == 0) {
        cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
      } else {
        Rf_error("Unknown fill-rule: '%s'", fill_rule);
      }
    } else {
      Rf_warning("apply_params(): Unknown parameter: '%s'", nm);
    }
  }
  
  
  UNPROTECT(nprotect);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// R interface
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_params_set_(SEXP cr_, SEXP params_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cr_params_set(cr, params_);
  
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// R interface
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_params_save_(SEXP cr_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// R interface
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_params_restore_(SEXP cr_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_restore(cr);
  
  return cr_;
}






