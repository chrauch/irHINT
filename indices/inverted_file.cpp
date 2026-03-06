/******************************************************************************
 * Project:  irhint
 * Purpose:  Fast indexing for termporal information retrieval
 * Author:   Panagiotis Bouros, pbour@github.io
 * Author:   Christian Rauch
 ******************************************************************************
 * Copyright (c) 2023 - 2024
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#include "inverted_file.h"



#define INTERSECTION_STRATEGY_MERGESORT
//#define INTERSECTION_STRATEGY_BINARYSEARCH_SHRINK


InvertedFile::InvertedFile()
{
}


// Undecided if we should use two passes
InvertedFile::InvertedFile(const IRelation &R) : InvertedFileTemplate()
{
    this->R = R;

    // Step 1: one pass to count the contents inside each partition.
//    TermId *lsizes = new TermId[this->iR->dictionarySize]; // Counters
//
//    memset(lsizes, 0, sizeof(TermId));
//    for (auto &r : iR)
//    {
//        for (auto &tid : r.terms)
//            lsizes[tid]++;
//    }
//
//
//    // Step 2: allocate necessary memory
//    for (TermId tid = 0; tid < this->iR->dictionarySize; tid++)
//    {
//        if (lsizes[tid] > 0)
//            this->lists[tid].reserve(lsizes[tid]);
//    }
//    delete[] lsizes;

    
    // Step 3: index records
    for (auto &r : R)
        this->index(r);
        
}


void InvertedFile::index(const IRecord &r)
{
    for (auto &tid : r.terms)
        this->lists[tid].emplace_back(r.id);
}


void InvertedFile::getStats()
{
    size_t sum = 0;

    this->numEmptyLists = this->R.dictionarySize - this->lists.size();
    for (auto iter = this->lists.begin(); iter != this->lists.end(); iter++)
        sum += iter->second.size();
    this->avgListSize = (float)sum/this->lists.size();
}


size_t InvertedFile::getSize()
{
    size_t res = 0;
    
    for (auto iter = this->lists.begin(); iter != this->lists.end(); iter++)
        res += iter->second.size() * sizeof(RecordId);
    
    return res;
}


InvertedFile::~InvertedFile()
{
}


void InvertedFile::print(char c)
{
    for (auto iter = this->lists.begin(); iter != this->lists.end(); iter++)
    {
        cout << "t" << iter->first << " (" << iter->second.size() << "):";
        for (auto &rid : iter->second)
            cout << " " << c << rid;
        cout << endl;
    }
}


bool InvertedFile::moveOut(const RangeIRQuery &q, RelationId &candidates)
{
    auto iterE = this->lists.find(q.terms[0]);
    
    // If the inverted file does not contain the term then result must be empty
    if (iterE == this->lists.end())
        return false;
    else
    {
        for (const RecordId &rid: iterE->second)
                candidates.push_back(rid);
        
        return true;
    }
}


bool InvertedFile::intersect(const RangeIRQuery &q, const unsigned int off, RelationId &candidates)
{
    auto iterE = this->lists.find(q.terms[off]);
    
    
    if (iterE == this->lists.end())
        return false;
    else
    {
#ifdef INTERSECTION_STRATEGY_MERGESORT
        auto iterC    = candidates.begin();
        auto iterCEnd = candidates.end();
        auto iterL    = iterE->second.begin();
        auto iterLEnd = iterE->second.end();
        RelationId tmp;

        tmp.reserve(candidates.size());
        while ((iterC != iterCEnd) && (iterL != iterLEnd))
        {
            if (*iterL < *iterC)
                iterL++;
            else if (*iterL > *iterC)
                iterC++;
            else
            {
                tmp.push_back(*iterL);
                iterL++;
            }
        }
        candidates.swap(tmp);
#else if defined(INTERSECTION_STRATEGY_BINARYSEARCH_SHRINK)
//TODO
#endif
        
        return (!candidates.empty());
    }
}


void InvertedFile::intersectAndOutput(const RangeIRQuery &q, const unsigned int off, RelationId &candidates, RelationId &result)
{
    auto iterE = this->lists.find(q.terms[off]);
    
    
    if (iterE == this->lists.end())
        return;
    else
    {
#ifdef INTERSECTION_STRATEGY_MERGESORT
        auto iterC    = candidates.begin();
        auto iterCEnd = candidates.end();
        auto iterL    = iterE->second.begin();
        auto iterLEnd = iterE->second.end();

        while ((iterC != iterCEnd) && (iterL != iterLEnd))
        {
            if (*iterL < *iterC)
                iterL++;
            else if (*iterL > *iterC)
                iterC++;
            else
            {
                result.push_back(*iterL);
                iterL++;
            }
        }
#else if defined(INTERSECTION_STRATEGY_BINARYSEARCH_SHRINK)
//TODO
#endif
    }
}
