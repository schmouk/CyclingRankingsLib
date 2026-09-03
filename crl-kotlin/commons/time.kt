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

    override fun toString(): String = when (precision.toInt()) {
        0, 1 -> ""
        10 -> ".${value}"
        100 -> ".${value.toString().padStart(2, '0')}"
        1000 -> ".${value.toString().padStart(3, '0')}"
        else -> " ${value}/${precision}"
    }

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


//=====   Time Scores   ===================================
class Time {
    private var _seconds: Int = 0
    private var _fraction: SecondFraction = SecondFraction()
    private var _error_msg: String = ""

    constructor(other: Time) {
        _seconds = other._seconds
        _fraction = SecondFraction(other._fraction.value, other._fraction.precision)
        _error_msg = other._error_msg
    }

    constructor(h: UShort, m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort) {
        _evaluate_data(h, m, s, SecondFraction(frac_val, frac_prec))
    }

    constructor(h: UShort, m: UByte, s: UByte, frac: SecondFraction) {
        _evaluate_data(h, m, s, frac)
    }

    constructor(h: UShort, m: UByte, s: UByte) {
        _evaluate_data(h, m, s)
    }

    constructor(m: UByte, s: UByte, frac_val: UShort, frac_prec: UShort) {
        _evaluate_data(0u, m, s, SecondFraction(frac_val, frac_prec))
    }

    constructor(m: UByte, s: UByte, frac: SecondFraction) {
        _evaluate_data(0u, m, s, frac)
    }

    constructor(m: UByte, s: UByte) {
        _evaluate_data(0u, m, s)
    }

    constructor(s: UByte, frac_val: UShort, frac_prec: UShort) {
        _evaluate_data(0u, 0u, s, SecondFraction(frac_val, frac_prec))
    }

    constructor(s: UByte, frac: SecondFraction) {
        _evaluate_data(0u, 0u, s, frac)
    }

    constructor(s: UInt) {
        if (s < 60u) _evaluate_data(0u, 0u, s.toUByte())
        else _evaluate_data((s / 3600u).toUShort(), ((s % 3600u) / 60u).toUByte(), (s % 60u).toUByte())
    }

    constructor(time: Double, precision: Int = 0) {
        val integerPart = kotlin.math.floor(time)
        _seconds = integerPart.toInt()
        _fraction = if (precision == 0) {
            SecondFraction()
        } else {
            SecondFraction((precision * (time - integerPart)).toInt().toUShort(), precision.toUShort())
        }
    }

    constructor(time: String?) {
        if (time != null) _evaluate_time(time)
    }

    fun assign(time: String?): Time {
        _error_msg = ""
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
            _error_msg = ""
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
                _error_msg = ""
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
            hours > 0 -> "%d:%02d:%02d%s".format(hours, minutes, seconds, fraction)
            minutes > 0 -> "%2d:%02d%s".format(minutes, seconds, fraction)
            else -> "%2d%s".format(seconds, fraction)
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

    fun is_ok(): Boolean = _error_msg.isEmpty()

    fun get_error_message(): String = _error_msg

    private fun _evaluate_data(h: UShort, m: UByte, s: UByte, frac: SecondFraction) {
        when {
            m >= 60.toUByte() -> _error_msg = "bad value for minutes: $m"
            s >= 60.toUByte() -> _error_msg = "bad value for seconds: $s"
            else -> {
                _seconds = 3600 * h.toInt() + 60 * m.toInt() + s.toInt()
                _fraction = frac
                _error_msg = ""
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

    private fun _evaluate_time(str: String) {
        try {
            val hoursWithFraction = Regex("^(\\d+):(\\d{2}):(\\d{2}) (\\d+)/(\\d+)$").matchEntire(str)
            val hoursWithDecimal = Regex("^(\\d+):(\\d{2}):(\\d{2})(?:\\.(\\d+))?$").matchEntire(str)
            val minutesWithFraction = Regex("^(\\d{1,2}):(\\d{2}) (\\d+)/(\\d+)$").matchEntire(str)
            val minutesWithDecimal = Regex("^(\\d+):(\\d{2})(?:\\.(\\d+))?$").matchEntire(str)
            val secondsWithFraction = Regex("^(\\d{1,2}) (\\d+)/(\\d+)$").matchEntire(str)
            val secondsWithDecimal = Regex("^(\\d{1,2})(?:\\.(\\d+))?$").matchEntire(str)

            when {
                hoursWithFraction != null -> {
                    val groups = hoursWithFraction.groupValues
                    _seconds = 3600 * groups[1].toInt() + 60 * groups[2].toInt() + groups[3].toInt()
                    _evaluate_frac(groups[4], groups[5])
                }
                hoursWithDecimal != null -> {
                    val groups = hoursWithDecimal.groupValues
                    _seconds = 3600 * groups[1].toInt() + 60 * groups[2].toInt() + groups[3].toInt()
                    _evaluate_frac(groups[4])
                }
                minutesWithFraction != null -> {
                    val groups = minutesWithFraction.groupValues
                    _seconds = 60 * groups[1].toInt() + groups[2].toInt()
                    _evaluate_frac(groups[3], groups[4])
                }
                minutesWithDecimal != null -> {
                    val groups = minutesWithDecimal.groupValues
                    _seconds = 60 * groups[1].toInt() + groups[2].toInt()
                    _evaluate_frac(groups[3])
                }
                secondsWithFraction != null -> {
                    val groups = secondsWithFraction.groupValues
                    _seconds = groups[1].toInt()
                    _evaluate_frac(groups[2], groups[3])
                }
                secondsWithDecimal != null -> {
                    val groups = secondsWithDecimal.groupValues
                    _seconds = groups[1].toInt()
                    _evaluate_frac(groups[2])
                }
            }
        } catch (_: NumberFormatException) {
            _error_msg = "bad time value: $str"
        }
    }
}
