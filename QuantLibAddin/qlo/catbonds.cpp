/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2012, 2013  Grzegorz Andruszkiewicz

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#if defined(HAVE_CONFIG_H)
    #include <qlo/config.hpp>
#endif

#include <qlo/catbonds.hpp>
#include <ql/experimental/catbonds/all.hpp>

#include <oh/repository.hpp>

#include <ostream>

using std::vector;
using std::string;
using boost::shared_ptr;
using ObjectHandler::property_t;
using ObjectHandler::convert2;
using QuantLib::Size;
using QuantLib::Date;
using boost::dynamic_pointer_cast;

namespace QuantLibAddin {
    NotionalRisk::NotionalRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& p,
            bool permanent) 
    : ObjectHandler::LibraryObject<QuantLib::NotionalRisk>(p, permanent) {
    }

    DigitalNotionalRisk::DigitalNotionalRisk(const boost::shared_ptr<ObjectHandler::ValueObject>& p,
                                             QuantLib::Real threshold,
                                             bool permanent)
    : NotionalRisk(p, permanent) {
    
        boost::shared_ptr<QuantLib::EventPaymentOffset> paymentOffset(new QuantLib::NoOffset());

        libraryObject_ = boost::shared_ptr<QuantLib::DigitalNotionalRisk>(new
            QuantLib::DigitalNotionalRisk(paymentOffset,
                                    threshold));
    }

    ProportionalNotionalRisk::ProportionalNotionalRisk(const boost::shared_ptr<ObjectHandler::ValueObject>& p,
                                             QuantLib::Real attachement, 
                                             QuantLib::Real exhaustion,
                                             bool permanent)
    : NotionalRisk(p, permanent) {
    
        boost::shared_ptr<QuantLib::EventPaymentOffset> paymentOffset(new QuantLib::NoOffset());

        libraryObject_ = boost::shared_ptr<QuantLib::ProportionalNotionalRisk>(new
            QuantLib::ProportionalNotionalRisk(paymentOffset,
                                    attachement, exhaustion));
    }


    FloatingCatBond::FloatingCatBond(
            const shared_ptr<ObjectHandler::ValueObject>& properties,
            const string& des,
            const QuantLib::Currency& cur,
            QuantLib::Natural settlementDays,
            QuantLib::BusinessDayConvention paymentConvention,
            QuantLib::Real faceAmount,
            const shared_ptr<QuantLib::Schedule>& schedule,
            QuantLib::Natural fixingDays,
            bool inArrears,
            const QuantLib::DayCounter& paymentDayCounter,
            const vector<QuantLib::Rate>& floors,
            const vector<QuantLib::Real>& gearings,
            const shared_ptr<QuantLib::IborIndex>& index,
            const vector<QuantLib::Spread>& spreads,
            const vector<QuantLib::Rate>& caps,
            QuantLib::Real redemption,
            const Date& issueDate,
            boost::shared_ptr<QuantLib::NotionalRisk> &notionalRisk,
            bool permanent)
    : Bond(properties, des, cur, permanent)
    {
        qlCatBondObject_ = shared_ptr<QuantLib::FloatingCatBond>(new
            QuantLib::FloatingCatBond(settlementDays, faceAmount, *schedule,
                                       index, paymentDayCounter,
                                       notionalRisk, paymentConvention, 
                                       fixingDays,
                                       gearings, spreads,
                                       caps, floors,
                                       inArrears,
                                       redemption, issueDate));
        qlBondObject_ = qlCatBondObject_;
        libraryObject_ = qlBondObject_;
        if (description_.empty()) {
            std::ostringstream temp;
            temp << "FloatingCatBond ";
            temp << QuantLib::io::iso_date(qlBondObject_->maturityDate());
            temp << " " << index->name();
            if (spreads.size()==1)
                temp << " " << spreads[0]*10000 << "bp";
            else
                temp << " step spread";
            description_ = temp.str();
        }
    }

    void FloatingCatBond::setCatBondPricingEngine(const boost::shared_ptr<QuantLib::MonteCarloCatBondEngine>& e) {
        qlBondObject_->setPricingEngine(e);
    }

    QuantLib::Real FloatingCatBond::lossProbability() { return qlCatBondObject_->lossProbability(); }
    QuantLib::Real FloatingCatBond::expectedLoss() { return qlCatBondObject_->expectedLoss(); }
    QuantLib::Real FloatingCatBond::exhaustionProbability() { return qlCatBondObject_->exhaustionProbability(); }

    FloatingCatBond::FloatingCatBond(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const std::string& des,
            const QuantLib::Currency& cur,
            bool permanent)
    : Bond(properties, des, cur, permanent) {}


    CatRisk::CatRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& p,
            bool permanent) 
    : ObjectHandler::LibraryObject<QuantLib::CatRisk>(p, permanent) {
    }

    BetaRisk::BetaRisk(const boost::shared_ptr<ObjectHandler::ValueObject>& p,
                                             QuantLib::Real maxLoss, 
                                             QuantLib::Real years, 
                                             QuantLib::Real mean, 
                                             QuantLib::Real stdDev,
                                             bool permanent)
    : CatRisk(p, permanent) {    
        libraryObject_ = boost::shared_ptr<QuantLib::CatRisk>(new
            QuantLib::BetaRisk(maxLoss, years, mean, stdDev));
    }

    MonteCarloCatBondEngine::MonteCarloCatBondEngine(
            const boost::shared_ptr<ObjectHandler::ValueObject>& p,
            const boost::shared_ptr<QuantLib::CatRisk> &catRisk,
            const QuantLib::Handle<QuantLib::YieldTermStructure>& discountCurve,
            bool permanent)
    : ObjectHandler::LibraryObject<QuantLib::MonteCarloCatBondEngine>(p, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::MonteCarloCatBondEngine>(new
            QuantLib::MonteCarloCatBondEngine(catRisk, discountCurve));
    }

}
