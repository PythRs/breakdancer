#include "io/Alignment.hpp"

#include <map>
#include <memory>

#include <gtest/gtest.h>

using namespace std;

namespace {
    uint8_t data[23] ={
        0x6a, 0x75, 0x6e, 0x6b, 0x0, 0x20, 0x0, 0x0, 0x0, 0x28, 0x27, 0x21,
        0x52, 0x47, 0x5a, 0x72, 0x67, 0x33, 0x0, 0x41, 0x4d, 0x43, 0x25
    };

    std::string expected_fastq(
        "@junk\n"
        "CT\n"
        "+\n"
        "HB\n"
        );
}

class TestAlignment : public ::testing::Test {
    protected:
        void SetUp() {
            core.tid = 22;
            core.pos = 29185299;
            core.bin = 6462;
            core.qual = 60;
            core.l_qname = 5;
            core.flag = 163;
            core.n_cigar = 1;
            core.l_qseq = 2;
            core.mtid = 22;
            core.mpos = 29184911;
            core.isize = -388;
            bam_record.core = core;
            bam_record.l_aux = 11;
            bam_record.data_len = 23;
            bam_record.m_data = 32;
            bam_record.data = &data[0];
            test_read.reset(new Alignment(&bam_record));
        }

        bam1_core_t core;
        bam1_t bam_record;
        auto_ptr<Alignment> test_read;
};

TEST_F(TestAlignment, readgroup) {
    ASSERT_EQ(test_read->readgroup(), "rg3");
}

TEST_F(TestAlignment, query_name) {
    ASSERT_EQ(test_read->query_name(), "junk");
}

TEST_F(TestAlignment, to_fastq) {
    ASSERT_TRUE(test_read->has_sequence());
    std::stringstream fq;
    test_read->to_fastq(fq);
    EXPECT_EQ(expected_fastq, fq.str());
}

TEST_F(TestAlignment, ori) {
    ASSERT_EQ(test_read->ori(), FWD);
}

TEST_F(TestAlignment, tid) {
    ASSERT_EQ(test_read->tid(), 22);
}

TEST_F(TestAlignment, pos) {
    ASSERT_EQ(test_read->pos(), 29185299);
}

TEST_F(TestAlignment, query_length) {
    ASSERT_EQ(test_read->query_length(), 2);
}

TEST_F(TestAlignment, leftmost) {
    ASSERT_FALSE(test_read->leftmost());
}

TEST_F(TestAlignment, abs_isize) {
    ASSERT_EQ(test_read->abs_isize(), 388);
}

TEST_F(TestAlignment, set_bdflag) {
    test_read->set_bdflag(ReadFlag::ARP_CTX);
    ASSERT_EQ(test_read->bdflag(), ReadFlag::ARP_CTX);
    test_read->set_bdflag(ReadFlag::NORMAL_FR);
}