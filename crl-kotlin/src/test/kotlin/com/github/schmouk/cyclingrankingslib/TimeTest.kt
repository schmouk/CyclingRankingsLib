package com.github.schmouk.cyclingrankingslib

import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class TimeTest {
    @Test
    fun secondFractionFormatsAndConvertsValues() {
        assertEquals(0.0, SecondFraction().toDouble(), 0.0)
        assertEquals("", SecondFraction(0u, 1u).toString())
        assertEquals(".7", SecondFraction(7u, 10u).toString())
        assertEquals(".07", SecondFraction(7u, 100u).toString())
        assertEquals(".007", SecondFraction(7u, 1000u).toString())
        assertEquals(" 7/11", SecondFraction(7u, 11u).toString())
        assertEquals(1.0, SecondFraction(1u, 2u).also { it += SecondFraction(1u, 2u) }.toDouble(), 0.0)
    }

    @Test
    fun secondFractionAddsFractionsWithDifferentPrecisions() {
        val fraction = SecondFraction(5u, 10u)

        fraction += SecondFraction(25u, 100u)

        assertEquals(3u.toUShort(), fraction.value)
        assertEquals(4u.toUShort(), fraction.precision)
        assertEquals(0.75, fraction.toDouble(), 0.0)
    }

    @Test
    fun secondFractionSupportsCxxComparisonSemantics() {
        val oneHalf = SecondFraction(1u, 2u)
        val threeQuarters = SecondFraction(3u, 4u)
        val closeValue = SecondFraction(500u, 1001u)

        assertTrue(oneHalf < threeQuarters)
        assertFalse(threeQuarters < oneHalf)
        assertTrue(oneHalf == SecondFraction(1u, 2u))
        assertTrue(oneHalf == closeValue)
        assertFalse(oneHalf == threeQuarters)
    }

    @Test
    fun constructorsCreateExpectedTimes() {
        assertEquals(3723L, Time(1u.toUShort(), 2u.toUByte(), 3u.toUByte()).toLong())
        assertEquals(3723.25, Time(1u.toUShort(), 2u.toUByte(), 3u.toUByte(), 25u.toUShort(), 100u.toUShort()).toDouble(), 0.0)
        assertEquals(123L, Time(2u.toUByte(), 3u.toUByte()).toLong())
        assertEquals(12L, Time(12u).toLong())
        assertEquals(3723L, Time(3723u).toLong())
        assertEquals(3723.5, Time(3723.5, 10).toDouble(), 0.0)
        assertEquals(3723L, Time(Time(1u.toUShort(), 2u.toUByte(), 3u.toUByte())).toLong())
        assertEquals(3723uL, Time(1u.toUShort(), 2u.toUByte(), 3u.toUByte()).toULong())
    }

    @Test
    fun stringParsingSupportsHoursMinutesSecondsAndFractions() {
        assertEquals(3723.125, Time("1:02:03.125").toDouble(), 0.0)
        assertEquals(3723.25, Time("1:02:03 1/4").toDouble(), 0.0)
        assertEquals(123.45, Time("2:03.45").toDouble(), 0.0)
        assertEquals(123.5, Time("2:03 1/2").toDouble(), 0.0)
        assertEquals(12.34, Time("12.34").toDouble(), 0.0)
        assertEquals(12.5, Time("12 1/2").toDouble(), 0.0)
        assertEquals(12.345, Time("12.34567").toDouble(), 0.0)
        assertEquals(" 5", Time("5").toString())
    }

    @Test
    fun stringFormattingUsesTheExpectedPrecisionAndWidths() {
        assertEquals("1:02:03.25", Time("1:02:03.25").toString())
        assertEquals(" 2:03.5", Time("2:03.5").toString())
        assertEquals(" 3.50", Time("3.50").toString())
        assertEquals(" 4 7/11", Time("4 7/11").toString())
    }

    @Test
    fun invalidDataAndNullValuesAreReported() {
        val invalidMinutes = Time(0u.toUShort(), 60u.toUByte(), 0u.toUByte())
        val invalidSeconds = Time(0u.toUShort(), 0u.toUByte(), 60u.toUByte())
        val zeroPrecision = Time("1:02:03 1/0")
        val invalidText = Time("not a time")

        assertFalse(invalidMinutes.is_ok())
        assertEquals("bad value for minutes: 60", invalidMinutes.get_error_message())
        assertFalse(invalidSeconds.is_ok())
        assertEquals("bad value for seconds: 60", invalidSeconds.get_error_message())
        assertFalse(zeroPrecision.is_ok())
        assertEquals("bad value for precision on fractions of seconds: 0", zeroPrecision.get_error_message())
        assertFalse(invalidText.is_ok())
        assertEquals("bad time string: not a time", invalidText.get_error_message())
        assertFalse(Time("999999999999999999999:00:00").is_ok())
        assertTrue(Time(null).is_ok())
        assertEquals(0L, Time(null).toLong())
    }

    @Test
    fun assignResetsPreviousState() {
        val time = Time("1:02:03.4")

        assertTrue(time.assign(null).is_ok())
        assertEquals(0.0, time.toDouble(), 0.0)
        assertEquals(" 0", time.toString())
        assertEquals(12.5, time.assign("12.5").toDouble(), 0.0)
    }

    @Test
    fun additionAndSubtractionHandleFractionsAndErrors() {
        val sum = Time("1:00:00.5") + Time("0:00:00.75")
        val difference = Time("1:00:00.75") - Time("0:00:00.5")

        assertEquals(3601.25, sum.toDouble(), 0.0)
        assertEquals(3600.25, difference.toDouble(), 0.0)
        assertEquals(3600.25, (Time("0:00:00.5") - Time("1:00:00.75")).toDouble(), 0.0)

        val mutable = Time("10")
        mutable -= Time("3")
        assertEquals(7L, mutable.toLong())
        mutable -= Time("9")
        assertFalse(mutable.is_ok())
        assertEquals(0L, mutable.toLong())

        val invalid = Time("not a time")
        val valid = Time("2")
        invalid += valid
        assertFalse(invalid.is_ok())
        valid += invalid
        assertFalse(valid.is_ok())
        assertFalse((valid + invalid).is_ok())
        assertFalse((valid - invalid).is_ok())
    }

    @Test
    fun timeSupportsAllComparisons() {
        val earlier = Time("1:02:03.4")
        val later = Time("1:02:03.5")
        val same = Time("1:02:03.4")
        val differentSeconds = Time("1:02:04")

        assertTrue(earlier == same)
        assertTrue(earlier != later)
        assertTrue(earlier < later)
        assertTrue(earlier <= same)
        assertTrue(earlier <= later)
        assertTrue(later > earlier)
        assertTrue(later >= same)
        assertTrue(differentSeconds > later)
        assertTrue(differentSeconds >= later)
        assertFalse(later < earlier)
        assertFalse(later <= earlier)
        assertFalse(earlier > later)
        assertFalse(earlier >= later)
    }
}