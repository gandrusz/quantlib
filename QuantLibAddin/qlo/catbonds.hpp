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

#ifndef qla_catbonds_hpp
#define qla_catbonds_hpp

#include "bonds.hpp"

namespace QuantLib {
    class CatBond;
    class CatRisk;
    class NotionalRisk;
    class DigitalNotionalRisk;
    class ProportionalNotionalRisk;
    class MonteCarloCatBondEngine;

    template <class T>
    class Handle;
    class YieldTermStructure;
}

namespace QuantLibAddin {
    class NotionalRisk : public ObjectHandler::LibraryObject<QuantLib::NotionalRisk> {
      public:
        NotionalRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            bool permanent);
    };

    class DigitalNotionalRisk : public NotionalRisk {
      public:
        DigitalNotionalRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            QuantLib::Real threshold,
            bool permanent);
    };

    class ProportionalNotionalRisk : public NotionalRisk {
      public:
        ProportionalNotionalRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            QuantLib::Real attachement, 
            QuantLib::Real exhaustion,
            bool permanent);
    };

    class FloatingCatBond : public Bond {
      public:
        FloatingCatBond(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const std::string& des,
            const QuantLib::Currency& cur,
            QuantLib::Natural settlementDays,
            QuantLib::BusinessDayConvention paymentConvention,
            QuantLib::Real faceAmount,
            const boost::shared_ptr<QuantLib::Schedule>& schedule,
            QuantLib::Natural fixingDays,
            bool inArrears,
            const QuantLib::DayCounter& paymentDayCounter,
            const std::vector<QuantLib::Rate>& floors,
            const std::vector<QuantLib::Real>& gearings,
            const boost::shared_ptr<QuantLib::IborIndex>& index,
            const std::vector<QuantLib::Spread>& spreads,
            const std::vector<QuantLib::Rate>& caps,
            QuantLib::Real redemption,
            const QuantLib::Date& issueDate,
            boost::shared_ptr<QuantLib::NotionalRisk> &notionalRisk,
            bool permanent           
            );
        
        void setCatBondPricingEngine(const boost::shared_ptr<QuantLib::MonteCarloCatBondEngine>& e);

        QuantLib::Real lossProbability();
        QuantLib::Real expectedLoss();
        QuantLib::Real exhaustionProbability();

		QuantLib::Real var();
		QuantLib::Real stdDev();
		QuantLib::Real skew();
		QuantLib::Real kurtosis();

      protected:
        FloatingCatBond(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const std::string& description,
            const QuantLib::Currency& currency,
            bool permanent);

        boost::shared_ptr<QuantLib::CatBond> qlCatBondObject_;
    };

    class CatRisk : public ObjectHandler::LibraryObject<QuantLib::CatRisk> {
      public:
        CatRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            bool permanent);
    };

    class BetaRisk : public CatRisk {
      public:
        BetaRisk(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            QuantLib::Real maxLoss, 
            QuantLib::Real years, 
            QuantLib::Real mean, 
            QuantLib::Real stdDev,
            bool permanent);
    };

    class MonteCarloCatBondEngine : public ObjectHandler::LibraryObject<QuantLib::MonteCarloCatBondEngine> {
      public:
        MonteCarloCatBondEngine(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const boost::shared_ptr<QuantLib::CatRisk> &catRisk,
            const QuantLib::Handle<QuantLib::YieldTermStructure>& discountCurve,
			QuantLib::Real varLevel,
            bool permanent);
    };
}

#endif