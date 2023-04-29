(defconst cosmo-c-constants-c11
 '("__func__"
   "__VA_ARGS__"
   "__STDC__"
   "__STDC_HOSTED__"
   "__STDC_VERSION__"
   "__TIME__"
   "__STDC_ISO_10646__"
   "__STDC_MB_MIGHT_NEQ_WC__"
   "__STDC_UTF_16__"
   "__STDC_UTF_32__"
   "__STDC_ANALYZABLE__"
   "__STDC_IEC_559_COMPLEX__"
   "__STDC_LIB_EXT1__"
   "__STDC_NO_ATOMICS__"
   "__STDC_NO_COMPLEX__"
   "__STDC_NO_THREADS__"
   "__STDC_NO_VLA__"
   "__STDC_WANT_LIB_EXT1__"))

(defconst cosmo-c-constants-limits
  '("IMAGE_BASE_VIRTUAL"
    "IMAGE_BASE_REAL"
    "IMAGE_BASE_PHYSICAL"
    "SIZEOF_SHORT"
    "SIZEOF_INT"
    "SIZEOF_LONG"
    "SIZEOF_LONG_LONG"
    "SIZEOF_POINTER"
    "SIZEOF_PTRDIFF_T"
    "SIZEOF_SIZE_T"
    "SIZEOF_WCHAR_T"
    "SIZEOF_WINT_T"
    "SIZEOF_FLOAT"
    "SIZEOF_FLOAT128"
    "SIZEOF_DOUBLE"
    "SIZEOF_FLOAT80"
    "SIZEOF_LONG_DOUBLE"
    "SIZEOF_INTMAX"
    "SCHAR_MAX"
    "SHRT_MAX"
    "INT_MAX"
    "LONG_MAX"
    "LLONG_MAX"
    "LONG_LONG_MAX"
    "SIZE_MAX"
    "INT8_MAX"
    "INT16_MAX"
    "INT32_MAX"
    "INT64_MAX"
    "INT128_MAX"
    "WINT_MAX"
    "WCHAR_MAX"
    "INTPTR_MAX"
    "PTRDIFF_MAX"
    "SCHAR_MIN"
    "SHRT_MIN"
    "UINT_MIN"
    "INT_MIN"
    "LONG_MIN"
    "LLONG_MIN"
    "LONG_LONG_MIN"
    "SIZE_MIN"
    "INT8_MIN"
    "INT16_MIN"
    "INT32_MIN"
    "INT64_MIN"
    "INT128_MIN"
    "INTMAX_MIN"
    "INTPTR_MIN"
    "WINT_MIN"
    "WCHAR_MIN"
    "PTRDIFF_MIN"
    "USHRT_MAX"
    "UINT_MAX"
    "ULONG_MAX"
    "ULLONG_MAX"
    "ULONG_LONG_MAX"
    "UINTPTR_MAX"
    "UINT8_MAX"
    "UINT16_MAX"
    "UINT32_MAX"
    "UINT64_MAX"
    "UINT128_MAX"
    "USHRT_MIN"
    "ULONG_MIN"
    "ULLONG_MIN"
    "ULONG_LONG_MIN"
    "UINT8_MIN"
    "UINT16_MIN"
    "UINT32_MIN"
    "UINT64_MIN"
    "UINT128_MIN"
    "UINTMAX_MIN"
    "UINTPTR_MIN"
    "MB_CUR_MAX"
    "MB_LEN_MAX"
    "INTMAX_MAX"
    "UINTMAX_MAX"
    "INTMAX_MAX"
    "UINTMAX_MAX"
    "DBL_MIN"
    "DBL_MAX"
    "FLT_MIN"
    "FLT_MAX"))

(defconst cosmo-c-constants-regex
  (concat "\\_<"
          (regexp-opt (append cosmo-c-constants-c11
                              cosmo-c-constants-limits))
          "\\_>"))

(provide 'cosmo-c-constants)
