package com.github.schmouk.cyclingrankingslib

/**
* This file is part of project CyclingRankingsLibrary
* (https://github.com/schmouk/CyclingRankingsLib)
*
* Copyright (C) 2026 Philippe Schmouker
*
* License is GNU GENERAL PUBLIC LICENSE Version 3
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program (see:
* https://github.com/schmouk/CyclingRankingsLib/blob/main/LICENSE).
* If not, see <https://www.gnu.org/licenses/>.
*/

//=====   Time Fraction of Seconds   ======================
class SecondFraction(
    var value: UShort = 0u,
    var precision: UShort = 1u
) {
    operator fun plusAssign(other: SecondFraction) {
        if (precision == other.precision) {
            value = (value.toUInt() + other.value.toUInt()).toUShort()
        } else {
            val commonDenominator = _lcm(precision.toUInt(), other.precision.toUInt())
            val numerator = value.toUInt() * (commonDenominator / precision.toUInt()) +
                other.value.toUInt() * (commonDenominator / other.precision.toUInt())
            val gcd = _gcd(numerator, commonDenominator)
            value = (numerator / gcd).toUShort()
            precision = (commonDenominator / gcd).toUShort()
        }
    }

    fun toDouble(): Double = if (precision == 0.toUShort()) 0.0 else value.toDouble() / precision.toDouble()

    operator fun compareTo(other: SecondFraction): Int {
        if (precision == other.precision) return value.compareTo(other.value)
        return when {
            toDouble() < other.toDouble() - 0.001 -> -1
            this == other -> 0
            else -> 1
        }
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is SecondFraction) return false
        if (precision == other.precision) return value == other.value
        return kotlin.math.abs(toDouble() - other.toDouble()) < 0.001
    }

    override fun hashCode(): Int = 0

    override fun toString(): String = when (precision.toInt()) {
        0, 1 -> ""
        10 -> value.toString()
        100 -> value.toString().padStart(2, '0')
        1000 -> value.toString().padStart(3, '0')
        else -> "${value}/${precision}"
    }

    fun isDefault(): Boolean = value == 0.toUShort() && precision == 1.toUShort()

    private fun _gcd(first: UInt, second: UInt): UInt {
        var a = first
        var b = second
        while (b != 0u) {
            val temporary = b
            b = a % b
            a = temporary
        }
        return a
    }

    private fun _lcm(first: UInt, second: UInt): UInt = (first / _gcd(first, second)) * second
}


//=====   Localization of time separators   ===============
data class LocalTimeSeps(
    val hSep: Char = ':',
    val mSep: Char = ':',
    val sSep: Char = '.',
    val forceSecSep: Boolean = false
) {
    constructor(timeSeps: String, forceSecSep: Boolean = false) : this(
        timeSeps.getOrElse(0) { ':' },
        timeSeps.getOrElse(1) { ':' },
        timeSeps.getOrElse(2) { '.' },
        forceSecSep
    )
}

val InternationalTimeSeps = LocalTimeSeps("::.")
val DutchTimeSeps = LocalTimeSeps("um,")
val EuropeanTimeSeps = LocalTimeSeps("::,")
val FrenchTimeSeps = LocalTimeSeps("h'\"", true)


//=====   Time Scores   ===================================
open class Time {
    protected var _local: LocalTimeSeps = InternationalTimeSeps
    protected var _seconds: Int = 0
    protected var _fraction: SecondFraction = SecondFraction()
    protected var _error_msg: String = ""

    constructor()

    constructor(localize: LocalTimeSeps) {
        _local = localize
    }

    constructor(other: Time) {
        _local = other._local
        _seconds = other._seconds
        _fraction = SecondFraction(other._fraction.value, other._fraction.precision)
        _error_msg = other._error_msg
    }

    constructor(h: UShort, m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        _evaluate_data(h, m, s, SecondFraction(frac_val, frac_prec))
    }

    constructor(h: UShort, m: UByte, s: UByte, frac: SecondFraction, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        _evaluate_data(h, m, s, frac)
    }

    constructor(h: UShort, m: UByte, s: UByte, localize: LocalTimeSeps) : this(localize) {
        _evaluate_data(h, m, s)
    }

    constructor(h: UShort, m: UByte, s: UByte) : this(h, m, s, InternationalTimeSeps)

    constructor(m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort) {
        _evaluate_data(0u, m, s, SecondFraction(frac_val, frac_prec))
    }

    constructor(m: UByte, s: UByte, frac: SecondFraction) {
        _evaluate_data(0u, m, s, frac)
    }

    constructor(m: UByte, s: UByte, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        _evaluate_data(0u, m, s)
    }

    constructor(s: UByte, frac_val: UShort, frac_prec: UShort, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        _evaluate_data(0u, 0u, s, SecondFraction(frac_val, frac_prec))
    }

    constructor(s: UByte, frac: SecondFraction, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        _evaluate_data(0u, 0u, s, frac)
    }

    constructor(s: UInt, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        if (s < 60u) _evaluate_data(0u, 0u, s.toUByte())
        else _evaluate_data((s / 3600u).toUShort(), ((s % 3600u) / 60u).toUByte(), (s % 60u).toUByte())
    }

    constructor(time: Double, precision: Int, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        val integerPart = kotlin.math.floor(time)
        _seconds = integerPart.toInt()
        _fraction = if (precision == 0) {
            SecondFraction()
        } else {
            SecondFraction(kotlin.math.round(precision * (time - integerPart)).toInt().toUShort(), precision.toUShort())
        }
    }

    constructor(time: Double, localize: LocalTimeSeps) : this(localize) {
        _seconds = time.toInt()
    }

    constructor(time: Double) : this(time, 0, InternationalTimeSeps)

    constructor(time: String?, localize: LocalTimeSeps = InternationalTimeSeps) : this(localize) {
        if (time != null) _evaluate_time(time)
    }

    fun assign(time: String?): Time {
        clr_error()
        _seconds = 0
        _fraction = SecondFraction()
        if (time != null) _evaluate_time(time)
        return this
    }

    operator fun plusAssign(other: Time) {
        if (!is_ok()) {
            _error_msg = "attempt to add an erroneous time ($_error_msg) -> no addition"
        } else if (other.is_ok()) {
            _seconds += other._seconds
            clr_error()
            _fraction += other._fraction
        } else {
            _error_msg = "attempt to add an erroneous time (${other._error_msg}) -> no addition"
        }
    }

    operator fun minusAssign(other: Time) {
        if (!is_ok()) {
            _error_msg = "attempt to add an erroneous time ($_error_msg) -> no substraction"
        } else if (other.is_ok()) {
            if (other._seconds <= _seconds) {
                clr_error()
                _seconds -= other._seconds
            } else {
                _error_msg = "can't substract ${other._seconds} seconds from $_seconds seconds, result set to 0"
                _seconds = 0
            }
        } else {
            _error_msg = "attempt to substract an erroneous time (${other._error_msg}) -> no substraction"
        }
    }

    fun toLong(): Long = _seconds.toLong()
    fun toULong(): ULong = _seconds.toULong()
    fun toDouble(): Double = _seconds.toDouble() + _fraction.toDouble()

    override fun toString(): String {
        if (!is_ok()) return _error_msg
        val hours = _seconds / 3600
        val minutes = (_seconds % 3600) / 60
        val seconds = _seconds % 60
        val fraction = _fraction.toString()
        return when {
            hours > 0 -> "$hours${_local.hSep}${minutes.toString().padStart(2, '0')}${_local.mSep}${seconds.toString().padStart(2, '0')}${fractionSuffix(fraction)}"
            minutes > 0 -> "$minutes${_local.mSep}${seconds.toString().padStart(2, '0')}${fractionSuffix(fraction)}"
            else -> "$seconds${fractionSuffix(fraction)}"
        }
    }

    operator fun plus(other: Time): Time = Time(this).also { it += other }

    operator fun minus(other: Time): Time {
        if (!is_ok()) return this
        if (!other.is_ok()) return other
        val first = toDouble()
        val second = other.toDouble()
        val precision = maxOf(_fraction.precision.toInt(), other._fraction.precision.toInt())
        return Time(kotlin.math.abs(first - second), precision)
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is Time) return false
        return _seconds == other._seconds && _fraction == other._fraction
    }

    override fun hashCode(): Int = 31 * _seconds + _fraction.hashCode()

    operator fun compareTo(other: Time): Int {
        val secondsComparison = _seconds.compareTo(other._seconds)
        return if (secondsComparison != 0) secondsComparison else _fraction.compareTo(other._fraction)
    }

    fun is_ok(): Boolean = _error_msg.isEmpty()

    fun get_error_message(): String = _error_msg

    fun get_precision(): UShort = if (_fraction.precision > 0u) _fraction.precision else 1u

    fun clr_error() {
        _error_msg = ""
    }

    fun set_hms_sep(hSep: Char, mSep: Char, sSep: Char, forceSecSep: Boolean = false) {
        _local = LocalTimeSeps(hSep, mSep, sSep, forceSecSep)
    }

    fun set_hms_sep(timeSeps: String, forceSecSep: Boolean = false) {
        _local = LocalTimeSeps(timeSeps, forceSecSep)
    }

    fun set_hms_sep(local: LocalTimeSeps) {
        _local = local
    }

    protected fun fractionSuffix(fraction: String): String = if (fraction.isEmpty()) {
        if (_local.forceSecSep) _local.sSep.toString() else ""
    } else {
        "${_local.sSep}$fraction"
    }

    private fun _evaluate_data(h: UShort, m: UByte, s: UByte, frac: SecondFraction) {
        when {
            m >= 60.toUByte() -> _error_msg = "bad value for minutes: $m"
            s >= 60.toUByte() -> _error_msg = "bad value for seconds: $s"
            else -> {
                _seconds = 3600 * h.toInt() + 60 * m.toInt() + s.toInt()
                _fraction = frac
                clr_error()
            }
        }
    }

    private fun _evaluate_data(h: UShort, m: UByte, s: UByte) = _evaluate_data(h, m, s, SecondFraction())

    private fun _evaluate_frac(frac_str: String) {
        if (frac_str.isEmpty()) {
            _fraction = SecondFraction()
            return
        }
        val value = frac_str.toUShort()
        _fraction = when (frac_str.length) {
            1 -> SecondFraction(value, 10u)
            2 -> SecondFraction(value, 100u)
            3 -> SecondFraction(value, 1000u)
            else -> SecondFraction(frac_str.take(3).toUShort(), 1000u)
        }
    }

    private fun _evaluate_frac(frac_value: String, frac_precision: String) {
        _fraction.value = frac_value.toUShort()
        _fraction.precision = frac_precision.toUShort()
        if (_fraction.precision == 0.toUShort()) _error_msg = "bad value for precision on fractions of seconds: 0"
    }

    protected open fun _evaluate_time(str: String) {
        if (_evaluate_hms_ratio(str)) return
        if (_evaluate_hms_frac(str)) return
        if (_evaluate_ms_frac(str)) return
        if (_evaluate_s_frac(str)) return
        if (_evaluate_ms_ratio(str)) return
        if (_evaluate_s_ratio(str)) return

        _error_msg = "bad time string: $str"
    }

    private fun escaped(value: Char): String = Regex.escape(value.toString())

    private fun _evaluate_hms_ratio(str: String) : Boolean {
        try {
            clr_error()

            val hoursWithFraction = Regex("^(\\d+)${escaped(_local.hSep)}(\\d{2})${escaped(_local.mSep)}(\\d{2}) (\\d+)/(\\d+)$").matchEntire(str)
            if (hoursWithFraction != null) {
                val groups = hoursWithFraction.groupValues
                _seconds = 3600 * groups[1].toInt() + 60 * groups[2].toInt() + groups[3].toInt()
                _evaluate_frac(groups[4], groups[5])
                return true
            }
            else return false
        } catch (_: NumberFormatException) {
            _error_msg = "bad HMS time string: $str"
            return false
        }
    }

    private fun _evaluate_hms_frac(str: String) : Boolean {
        try {
            clr_error()

            val hoursWithDecimal = Regex("^(\\d+)${escaped(_local.hSep)}(\\d{2})${escaped(_local.mSep)}(\\d{2})(?:${escaped(_local.sSep)}(\\d+))?$").matchEntire(str)
            if (hoursWithDecimal != null) {
                val groups = hoursWithDecimal.groupValues
                _seconds = 3600 * groups[1].toInt() + 60 * groups[2].toInt() + groups[3].toInt()
                _evaluate_frac(groups[4])
                return true
            }
            else return false
        } catch (_: NumberFormatException) {
            _error_msg = "bad HMS time string: $str"
            return false
        }
    }

    protected fun _evaluate_ms_ratio(str: String) : Boolean {
        try {
            clr_error()

            val minutesWithFraction = Regex("^(\\d{1,2})${escaped(_local.mSep)}(\\d{2}) (\\d+)/(\\d+)$").matchEntire(str)
            if (minutesWithFraction != null) {
                val groups = minutesWithFraction.groupValues
                _seconds = 60 * groups[1].toInt() + groups[2].toInt()
                _evaluate_frac(groups[3], groups[4])
                return true
            }
            else return false
        } catch (_: NumberFormatException) {
            _error_msg = "bad MS time string: $str"
            return false
        }
    }

    protected fun _evaluate_ms_frac(str: String) : Boolean {
        try {
            clr_error()

            val minutesWithDecimal = Regex("^(\\d+)${escaped(_local.mSep)}(\\d{2})(?:${escaped(_local.sSep)}(\\d+))?$").matchEntire(str)
            if (minutesWithDecimal != null) {
                val groups = minutesWithDecimal.groupValues
                _seconds = 60 * groups[1].toInt() + groups[2].toInt()
                _evaluate_frac(groups[3])
                return true
            }
            else return false
        } catch (_: NumberFormatException) {
            _error_msg = "bad MS time string: $str"
            return false
        }
    }

    protected fun _evaluate_s_ratio(str: String) : Boolean {
        try {
            clr_error()

            val secondsWithFraction = Regex("^(\\d{1,2}) (\\d+)/(\\d+)$").matchEntire(str)
            if (secondsWithFraction != null) {
                val groups = secondsWithFraction.groupValues
                _seconds = groups[1].toInt()
                _evaluate_frac(groups[2], groups[3])
                return true
            }
            else return false
        } catch (_: NumberFormatException) {
            _error_msg = "bad S time vastringlue: $str"
            return false
        }
    }

    protected fun _evaluate_s_frac(str: String) : Boolean {
        try {
            clr_error()

            val secondsWithDecimal = Regex("^(\\d{1,2})(?:${escaped(_local.sSep)}(\\d+))?$").matchEntire(str)
            if (secondsWithDecimal != null) {
                val groups = secondsWithDecimal.groupValues
                _seconds = groups[1].toInt()
                _evaluate_frac(groups[2])
                return true
            }
            else return false
        } catch (_: NumberFormatException) {
            _error_msg = "bad S time string: $str"
            return false
        }
    }

}


//=====   HMSTime Scores   ================================
class HMSTime : Time {
    constructor(other: Time) : super(other)
    constructor(h: UShort, m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort) : super(h, m, s, frac_val, frac_prec)
    constructor(h: UShort, m: UByte, s: UByte, frac: SecondFraction) : super(h, m, s, frac)
    constructor(h: UShort, m: UByte, s: UByte) : super(h, m, s)
    constructor(m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort) : super(m, s, frac_val, frac_prec)
    constructor(m: UByte, s: UByte, frac: SecondFraction) : super(m, s, frac)
    constructor(m: UByte, s: UByte) : super(m, s)
    constructor(s: UByte, frac_val: UShort, frac_prec: UShort) : super(s, frac_val, frac_prec)
    constructor(s: UByte, frac: SecondFraction) : super(s, frac)
    constructor(s: UInt) : super(s)
    constructor(time: Double) : super(time)
    constructor(time: String?) : super(time)
}


//=====   HMTime Scores   =================================
class HMTime : Time {
    constructor(other: HMTime) : super(other)
    constructor(h: UShort, m: UByte, localize: LocalTimeSeps = InternationalTimeSeps) : super(h, m, 0u, localize)
    constructor(time: String?, localize: LocalTimeSeps = InternationalTimeSeps) : super(localize) {
        if (time != null) _evaluate_time(time)
    }

    override fun toString(): String {
        val hours = _seconds / 3600
        val minutes = (_seconds % 3600) / 60
        return if (_local.forceSecSep) {
            "$hours${_local.hSep}${minutes.toString().padStart(2, '0')}${_local.mSep}"
        } else {
            "$hours${_local.hSep}${minutes.toString().padStart(2, '0')}"
        }
    }
    
    override fun _evaluate_time(str: String) {
        try {
            val hoursWithFraction = Regex("^(\\d+)${Regex.escape(_local.hSep.toString())}(\\d{2})$").matchEntire(str)
            if (hoursWithFraction != null) {
                val groups = hoursWithFraction.groupValues
                _seconds = 3600 * groups[1].toInt() + 60 * groups[2].toInt()
                clr_error()
            }
            else {
                _error_msg = "erroneous format for 'hours:minutes' time string: '$str'"
            }
        }
        catch (_: NumberFormatException) {
            _error_msg = "erroneous format for 'hours:minutes' time string: '$str'"
        }
    }
}


//=====   MSTime Scores   =================================
class MSTime : Time {
    constructor(other: MSTime) : super(other)
    constructor(m: UByte, s: UByte, localize: LocalTimeSeps = InternationalTimeSeps) : super(0u, m, s, localize)
    constructor(m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort, localize: LocalTimeSeps = InternationalTimeSeps) : super(0u, m, s, frac_val, frac_prec, localize)
    constructor(m: UByte, s: UByte, frac: SecondFraction, localize: LocalTimeSeps = InternationalTimeSeps) : super(0u, m, s, frac, localize)
    constructor(time: Double, precision: Int = 0, localize: LocalTimeSeps = InternationalTimeSeps) : super(time, precision, localize)
    constructor(time: String?, localize: LocalTimeSeps = InternationalTimeSeps) : super(time, localize)
    
    override fun _evaluate_time(str: String) {
        if (_evaluate_ms_frac(str)) return
        if (_evaluate_ms_ratio(str)) return
        
        _error_msg = "erroneous format for 'minutes:seconds + fraction' time string: '$str'"
    }
}


//=====   STime Scores   ==================================
class STime : Time {
    constructor(other: STime) : super(other)
    constructor(s: UByte, frac_val: UShort, frac_prec: UShort, localize: LocalTimeSeps = InternationalTimeSeps) : super(0u, 0u, s, frac_val, frac_prec, localize)
    constructor(s: UByte, frac: SecondFraction, localize: LocalTimeSeps = InternationalTimeSeps) : super(0u, 0u, s, frac, localize)
    constructor(s: UInt, localize: LocalTimeSeps = InternationalTimeSeps) : super(s.toDouble(), localize)
    constructor(time: Double, precision: Int = 0, localize: LocalTimeSeps = InternationalTimeSeps) : super(time, precision, localize)
    constructor(time: String?, localize: LocalTimeSeps = InternationalTimeSeps) : super(time, localize)
    
    override fun _evaluate_time(str: String) {
        if (_evaluate_s_frac(str)) return
        if (_evaluate_s_ratio(str)) return
        
        _error_msg = "erroneous format for 'seconds + fraction' time string: '$str'"
    }
}
