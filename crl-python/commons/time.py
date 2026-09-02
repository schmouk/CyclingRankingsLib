"""
This file is part of project CyclingRankingsLibrary
(https://github.com/schmouk/CyclingRankingsLib)

copyright (c) 2026 Philippe Schmouker

License is GNU GENERAL PUBLIC LICENSE Version 3
(full text available at:
 https://github.com/schmouk/CyclingRankingsLib?tab=GPL-3.0-1-ov-file)
"""

import re
#import math
import sys
from typing import Optional, Union


# ===== Time Fraction of Seconds ======================
class SecondFraction:
    """Represents a fraction of seconds for time calculations."""

    def __init__(self, value: int = 0, precision: int = 1) -> None:
        """Initialize a SecondFraction.

        Args:
            value: Numerator of the fraction (may be greater than precision)
            precision: Denominator of the fraction (mostly 5, 10, 100 or 1000)
        """
        self.value: int = value
        self.precision: int = precision

    def __float__(self) -> float:
        """Convert fraction to float."""
        try:
            return float(self.value) / float(self.precision)
        except:
            return 0.0

    def __str__(self) -> str:
        """Convert fraction to string representation."""
        match self.precision:
            case 0 | 1:
                return ""
            case 10:
                return f".{self.value}"
            case 100:
                return f".{self.value:02d}"
            case 1000:
                return f".{self.value:03d}"
            case _:
                return f" {self.value}/{self.precision}"

    def __iadd__(self, other: 'SecondFraction') -> 'SecondFraction':
        """Add another fraction to this fraction (+=)."""
        if self.precision == other.precision:
            self.value += other.value
        else:
            common_denominator = self._lcm(self.precision, other.precision)
            numerator = (
                self.value * (common_denominator // self.precision) +
                other.value * (common_denominator // other.precision)
            )
            gcd = self._gcd(numerator, common_denominator)
            self.value = numerator // gcd
            self.precision = common_denominator // gcd

        return self

    @staticmethod
    def _gcd(a: int, b: int) -> int:
        """Calculate greatest common divisor."""
        while b != 0:
            a, b = b, a % b
        return a

    @staticmethod
    def _lcm(a: int, b: int) -> int:
        """Calculate least common multiple."""
        return (a // SecondFraction._gcd(a, b)) * b


# ===== Time Scores ===================================
class Time:
    """Represents a time value with seconds and fractions."""

    def __init__(
        self,
        h: Optional[int] = None,
        m: Optional[int] = None,
        s: Optional[Union[int, float]] = None,
        frac_val: Optional[int] = None,
        frac_prec: Optional[int] = None
    ) -> None:
        """Initialize a Time object.

        Supports multiple constructor patterns:
        - Time(h, m, s, frac_val, frac_prec)
        - Time(h, m, s, SecondFraction)
        - Time(h, m, s)
        - Time(m, s, frac_val, frac_prec)
        - Time(m, s, SecondFraction)
        - Time(m, s)
        - Time(s, frac_val, frac_prec)
        - Time(s, SecondFraction)
        - Time(unsigned_int_s)
        - Time(double_time, precision)
        - Time(time_string)
        """
        self._seconds: int = 0
        self._fraction: SecondFraction = SecondFraction(0, 1)
        self._error_msg: str = ""

        # Determine which constructor pattern was used
        if h is None:
            # Empty constructor
            pass
        elif isinstance(h, str):
            # String constructor
            self._evaluate_time(h)
        elif isinstance(h, float):
            # Double constructor with precision
            self._init_from_double(h, m if m is not None else 0)
        elif m is None:
            # Single parameter: unsigned int seconds
            self._init_from_seconds(int(h))
        elif s is None:
            # Two parameters: m, s (no fraction)
            self._evaluate_data(0, int(h), int(m))
        elif isinstance(s, SecondFraction):
            # Pattern: Time(h, m, s_or_m, SecondFraction)
            if frac_val is None:  # This is Time(m, s, SecondFraction)
                self._evaluate_data(0, int(h), int(m), s)
            else:  # This is Time(h, m, s, SecondFraction)
                self._evaluate_data(int(h), int(m), int(frac_val), s)
        elif frac_val is not None and frac_prec is not None:
            # Pattern with frac_val and frac_prec
            if frac_prec is None:  # Time(s, frac_val, frac_prec)
                self._evaluate_data(0, 0, int(h), SecondFraction(int(m), int(s)))
            else:  # Time(h, m, s, frac_val, frac_prec) or Time(m, s, frac_val, frac_prec)
                # Determine if h is hours or minutes based on value patterns
                if isinstance(frac_val, int):
                    # For now assume it's Time(h, m, s, frac_val, frac_prec) pattern
                    self._evaluate_data(int(h), int(m), int(s), SecondFraction(int(frac_val), int(frac_prec)))
        else:
            # Pattern: Time(h, m, s) or Time(m, s) or Time(s)
            # Need to infer based on parameter magnitudes
            if isinstance(h, int) and m is not None and s is not None:
                if h > 255:  # Can't be uint8_t, must be hours
                    self._evaluate_data(h, int(m), int(s))
                elif m > 255:  # Can't be uint8_t for minutes
                    # time(s, frac_val, frac_prec) pattern
                    self._evaluate_data(0, 0, int(h), SecondFraction(int(m), int(s)))
                else:
                    # Could be (m, s) or (h, m, s), default to (h, m, s)
                    self._evaluate_data(int(h), int(m), int(s))

    def _init_from_double(self, time_val: float, precision: int) -> None:
        """Initialize from a double value with precision."""
        int_part = int(time_val)
        frac_part = time_val - int_part

        self._seconds = int_part
        if precision == 0:
            self._fraction = SecondFraction(0, 1)
        else:
            self._fraction = SecondFraction(
                int(precision * frac_part),
                precision
            )

    def _init_from_seconds(self, total_seconds: int) -> None:
        """Initialize from total seconds."""
        if total_seconds < 60:
            self._evaluate_data(0, 0, total_seconds)
        else:
            h = total_seconds // 3600
            m = (total_seconds % 3600) // 60
            s = total_seconds % 60
            self._evaluate_data(h, m, s)

    def __int__(self) -> int:
        """Convert to integer (long)."""
        if self._seconds <= sys.maxsize:
            return int(self._seconds)
        else:
            self._error_msg = (
                f"{self._seconds} is a too big time value to be converted to a "
                f"long integer, forced to {sys.maxsize}"
            )
            return sys.maxsize

    def __float__(self) -> float:
        """Convert to float."""
        return float(self._seconds) + float(self._fraction)

    def __str__(self) -> str:
        """Convert to string representation."""
        if not self.is_ok():
            return self._error_msg

        h = self._seconds // 3600
        m = (self._seconds % 3600) // 60
        s = self._seconds % 60
        frac = str(self._fraction)

        if h > 0:
            return f"{h}:{m:02d}:{s:02d}{frac}"

        if m > 0:
            return f"{m:2d}:{s:02d}{frac}"

        return f"{s:2d}{frac}"

    def __add__(self, other: 'Time') -> 'Time':
        """Add two times."""
        result = Time()
        result._seconds = self._seconds + other._seconds
        result._fraction = SecondFraction(self._fraction.value, self._fraction.precision)
        result._fraction += other._fraction
        result._error_msg = self._error_msg if self._error_msg else other._error_msg
        return result

    def __sub__(self, other: 'Time') -> 'Time':
        """Subtract two times (evaluates gaps, always positive)."""
        if not self.is_ok():
            return Time()  # Return empty Time

        if not other.is_ok():
            return Time()  # Return empty Time

        t1 = float(self)
        t2 = float(other)

        final_precision = max(self._fraction.precision, other._fraction.precision)

        if t1 <= t2:
            return Time(t2 - t1, final_precision)
        else:
            return Time(t1 - t2, final_precision)

    def __iadd__(self, other: 'Time') -> 'Time':
        """Add another time to this time (+=)."""
        if not self.is_ok():
            self._error_msg = f"attempt to add an erroneous time ({self._error_msg}) -> no addition"
        elif other.is_ok():
            # Special operation: fractions are added, but not to seconds
            self._seconds += other._seconds
            self._error_msg = ""
            self._fraction += other._fraction
        else:
            self._error_msg = f"attempt to add an erroneous time ({other._error_msg}) -> no addition"

        return self

    def __isub__(self, other: 'Time') -> 'Time':
        """Subtract another time from this time (-=)."""
        if not self.is_ok():
            self._error_msg = f"attempt to add an erroneous time ({self._error_msg}) -> no substraction"
        elif other.is_ok():
            # Substraction involves only seconds, not fractions
            if other._seconds <= self._seconds:
                self._error_msg = ""
                self._seconds -= other._seconds
            else:
                self._error_msg = (
                    f"can't substract {other._seconds} seconds from {self._seconds} "
                    f"seconds, result set to 0"
                )
                self._seconds = 0
        else:
            self._error_msg = f"attempt to substract an erroneous time ({other._error_msg}) -> no substraction"

        return self

    def is_ok(self) -> bool:
        """Check if this Time object is valid (no error)."""
        return len(self._error_msg) == 0

    def get_error_message(self) -> str:
        """Get the error message if any."""
        return self._error_msg

    def _evaluate_data(
        self,
        h: int,
        m: int,
        s: int,
        frac: Optional[SecondFraction] = None
    ) -> None:
        """Evaluate and set time data from h, m, s and optional fraction."""
        if m >= 60:
            self._error_msg = f"bad value for minutes: {m}"
        elif s >= 60:
            self._error_msg = f"bad value for seconds: {s}"
        else:
            self._seconds = 3600 * h + 60 * m + s
            self._fraction = frac if frac is not None else SecondFraction(0, 1)
            self._error_msg = ""

    def _evaluate_frac(self, frac_str: str, frac_precision: Optional[str] = None) -> None:
        """Evaluate fraction from string representation."""
        if not frac_str:
            self._fraction = SecondFraction(0, 1)
            return

        if frac_precision is not None:
            # Pattern: "value/precision"
            self._fraction.value = int(frac_str)
            self._fraction.precision = int(frac_precision)
        else:
            # Pattern: ".digits"
            frac_value = int(frac_str)
            match len(frac_str):
                case 1:
                    self._fraction = SecondFraction(frac_value, 10)
                case 2:
                    self._fraction = SecondFraction(frac_value, 100)
                case 3:
                    self._fraction = SecondFraction(frac_value, 1000)
                case _:
                    self._fraction = SecondFraction(int(frac_str[:3]), 1000)

    def _evaluate_time(self, time_str: str) -> None:
        """Evaluate time from string representation.

        Supports formats:
        - HHH:MM:SS frac_val/frac_precision
        - HHH:MM:SS.frac
        - MM:SS frac_val/frac_precision
        - MM:SS.frac
        - SS frac_val/frac_precision
        - SS.frac
        """
        # HHH:MM:SS frac_val/frac_precision
        match = re.search(r'^(\d\d*):(\d\d):(\d\d) (\d+)/(\d+)', time_str)
        if match:
            h = int(match.group(1))
            m = int(match.group(2))
            s = int(match.group(3))
            self._seconds = 3600 * h + 60 * m + s
            self._evaluate_frac(match.group(4), match.group(5))
            return

        # HHH:MM:SS.frac
        match = re.search(r'^(\d\d*):(\d\d):(\d\d)(\.(\d+))?', time_str)
        if match:
            h = int(match.group(1))
            m = int(match.group(2))
            s = int(match.group(3))
            self._seconds = 3600 * h + 60 * m + s
            self._evaluate_frac(match.group(5) if match.group(5) else "")
            return

        # MM:SS frac_val/frac_precision
        match = re.search(r'^(\d\d?):(\d\d) (\d+)/(\d+)', time_str)
        if match:
            m = int(match.group(1))
            s = int(match.group(2))
            self._seconds = 60 * m + s
            self._evaluate_frac(match.group(3), match.group(4))
            return

        # MM:SS.frac
        match = re.search(r'^(\d\d*):(\d\d)(\.(\d+))?', time_str)
        if match:
            m = int(match.group(1))
            s = int(match.group(2))
            self._seconds = 60 * m + s
            self._evaluate_frac(match.group(4) if match.group(4) else "")
            return

        # SS frac_val/frac_precision
        match = re.search(r'^(\d\d?) (\d+)/(\d+)', time_str)
        if match:
            s = int(match.group(1))
            self._seconds = s
            self._evaluate_frac(match.group(2), match.group(3))
            return

        # SS.frac
        match = re.search(r'^(\d\d?)(\.(\d+))?', time_str)
        if match:
            s = int(match.group(1))
            self._seconds = s
            self._evaluate_frac(match.group(3) if match.group(3) else "")
            return
