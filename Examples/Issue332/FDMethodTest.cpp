// CBS_implementation_MAIN.cpp : main file of the project
//						Tests of all the implementation

// The calculs need the use of iterators on QuantLib object (in the quantLib code himself)
//	This implies many vcc warning without real importance.
//	To disable this warning, use -D_SCL_SECURE_NO_WARNINGS in   
//		Project Properties > Configuration Proporties > C/C++ > command lines


#include <ql/quantlib.hpp>
#include <boost/foreach.hpp>
#include <iostream>

using namespace QuantLib;

int fdtest()
{
try{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++ General Parameter for all the computation +++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// declaration of the today's date (date where the records are done)
	Date		todaysDate (24 , Jan ,2012);			// INPUT
	Settings::instance().evaluationDate() = todaysDate; // /!\ IMPORTANT COMMAND REQUIRED FOR ALL VALUATIONS
	Calendar	calendar		= UnitedStates();		// INPUT
	Natural		settlementDays	= 2;					// INPUT
	// Calcul of the settlement date : need to add a period of 2 days to the todays date
	Date		settlementDate	= calendar.advance(todaysDate, Period(settlementDays, Days));
	DayCounter	dayCounter		= Actual360();			// INPUT
	Currency	currency		= USDCurrency();		// INPUT	

	std::cout << "Date of the evaluation:			" << todaysDate			<< std::endl;
	std::cout << "Calendar used:         			" << calendar.name()	<< std::endl;
	std::cout << "Number of settlement Days:		" << settlementDays		<< std::endl;
	std::cout << "Date of settlement:       		" << settlementDate		<< std::endl;
	std::cout << "Convention of day counter:		" << dayCounter.name()	<< std::endl;
	std::cout << "Currency of the actual context:\t\t" << currency.name()	<< std::endl;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++ Description of the underlying +++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	std::string		underlying_name		= "IBM";
	Real			underlying_price	= 191.75;	// INPUT
	Volatility		underlying_vol		= 0.2094;	// INPUT

	std::cout << "**********************************" << std::endl;
	std::cout << "Name of the underlying:			" << underlying_name	<< std::endl;
	std::cout << "Price of the underlying at t0:	" << underlying_price	<< std::endl;
	std::cout << "Volatility of the underlying:		" << underlying_vol		<< std::endl;

	// For a great managing of price and vol objects --> Handle
	Handle<Quote> underlying_priceH (boost::shared_ptr<Quote>(new SimpleQuote(underlying_price)));
	Handle<Quote> underlying_volH (boost::shared_ptr<Quote>(new SimpleQuote(underlying_vol)));

	// We suppose the vol constant : his term structure is flat --> BlackConstantVol object
	Handle<BlackVolTermStructure> flatVolTS( boost::shared_ptr<BlackVolTermStructure>(
						new BlackConstantVol(settlementDate,calendar, underlying_volH, dayCounter)));
	
	// ++++++++++++++++++++ Description of Yield Term Structure
	
	//  Libor data record 
	std::cout << "**********************************"	<< std::endl;
	std::cout << "Description of the Libor used for the Yield Curve construction" << std::endl;
	
	DayCounter Libor_dayCounter = Actual360();

	std::vector<Rate> liborRates;
	std::vector<Period> liborRatesTenor;
	// INPUT : all the following data are input : the rate and the corresponding tenor
	//		You could make the choice of more or less data
	//		--> However you have tho choice the instruments with different maturities
	liborRates.push_back(0.002763);//1M
	liborRatesTenor.push_back(Period(1,Months));
	liborRates.push_back(0.004082);//2M
	liborRatesTenor.push_back(Period(2,Months));
	liborRates.push_back(0.005601);//3M
	liborRatesTenor.push_back(Period(3,Months));
	liborRates.push_back(0.006390);//4M
	liborRatesTenor.push_back(Period(4,Months));
	liborRates.push_back(0.007125);//5M
	liborRatesTenor.push_back(Period(5,Months));
	liborRates.push_back(0.007928);//6M
	liborRatesTenor.push_back(Period(6,Months));
	liborRates.push_back(0.009446);//9M
	liborRatesTenor.push_back(Period(9,Months));
	liborRates.push_back(0.01110);//1Y
	liborRatesTenor.push_back(Period(1,Years));
	
	for(Size i=0; i<liborRates.size(); i++)
		std::cout << liborRatesTenor[i] << "\t\t\t" << liborRates[i] << std::endl;
	
	// Swap data record 

	//description of the fixed leg of the swap
	Period							Swap_fixedLegTenor		= Period(12, Months);				// INPUT
	BusinessDayConvention			Swap_fixedLegConvention = ModifiedFollowing;				// INPUT
	DayCounter						Swap_fixedLegDayCounter = Actual360();						// INPUT
	//description of the float leg of the swap
	boost::shared_ptr <IborIndex>	Swap_iborIndex (new Libor("USDLibor",Period(3,Months),settlementDays,USDCurrency(),
																UnitedStates(),Actual360()));	// INPUT

	std::cout << "Description of the Swap used for the Yield Curve construction"	<< std::endl;
	std::cout << "Tenor of the fixed leg:			" << Swap_fixedLegTenor			<< std::endl;
	std::cout << "Index of the floated leg: 		" << Swap_iborIndex->name()		<< std::endl;
	std::cout << "Maturity		Rate				" << std::endl;

	std::vector<Rate> swapRates;
	std::vector<Period> swapRatesTenor;
	// INPUT : all the following data are input : the rate and the corresponding tenor
	//		You could make the choice of more or less data
	//		--> However you have tho choice the instruments with different maturities
	swapRates.push_back(0.005681); // 2Y
	swapRatesTenor.push_back(Period(2,Years));
	swapRates.push_back(0.006970);// 3Y
	swapRatesTenor.push_back(Period(3,Years));
	swapRates.push_back(0.009310);//4Y
	swapRatesTenor.push_back(Period(4,Years));
	swapRates.push_back(0.012010);//5Y
	swapRatesTenor.push_back(Period(5,Years));
	swapRates.push_back(0.014628);//6Y
	swapRatesTenor.push_back(Period(6,Years));
	swapRates.push_back(0.016881);//7Y
	swapRatesTenor.push_back(Period(7,Years));
	swapRates.push_back(0.018745);//8Y
	swapRatesTenor.push_back(Period(8,Years));
	swapRates.push_back(0.020260);//9Y
	swapRatesTenor.push_back(Period(9,Years));
	swapRates.push_back(0.021545);//10Y
	swapRatesTenor.push_back(Period(10,Years));
	
	for(Size i=0; i<swapRates.size(); i++)
		std::cout << swapRatesTenor[i] << "\t\t\t" << swapRates[i] << std::endl;
	
// ++++++++++++++++++++ Creation of the vector of RateHelper (need for the Yield Curve construction)
	std::vector<boost::shared_ptr<RateHelper>> instruments;
// ++++++++++++++++++++ Libor 
	std::string LiborFamilyName = currency.name()+"Libor";	
	for (Size i=0; i<liborRates.size(); i++){
		//	Rate description ___ record of the rate	
		Handle<Quote> liborHandle(boost::shared_ptr<Quote> (new SimpleQuote(liborRates[i])));
		// Index description ___ creation of a Libor index
		boost::shared_ptr<IborIndex> liborIndex (new Libor(LiborFamilyName,liborRatesTenor[i],
																settlementDays,currency, 
																calendar,Libor_dayCounter) );
		// Initialize rate helper	___ the DepositRateHelper link the recording rate with the Libor index													
		instruments.push_back( boost::shared_ptr<RateHelper> (new DepositRateHelper(liborHandle,liborIndex)));
		}	
// +++++++++++++++++++++ Swap
	std::string SwapFamilyName = currency.name()+"swapIndex";
	for(Size i=0; i<swapRates.size();i++){
		//	Rate description ___ record of the rate		
		Handle<Quote> swapHandle ( boost::shared_ptr<Quote> (new SimpleQuote(swapRates[i])));
		//	swap description ___ creation of a swap index. The floating leg is described in the index 'Swap_iborIndex'
		boost::shared_ptr<SwapIndex> swapIndex ( new SwapIndex (SwapFamilyName, swapRatesTenor[i],
																	settlementDays, currency,
																	calendar, Swap_fixedLegTenor,
																	Swap_fixedLegConvention, Swap_fixedLegDayCounter,
																	Swap_iborIndex));
		// Initialize rate helper __ the SwapRateHelper links the swap index width his rate
		instruments.push_back(boost::shared_ptr<RateHelper> (new SwapRateHelper(swapHandle,swapIndex)));
	}
// ++++++++++++++++++  Now the creation of the yield curve
	Handle<YieldTermStructure> riskFreeTS ( boost::shared_ptr<YieldTermStructure> (
						new PiecewiseYieldCurve < ZeroYield,Linear>(settlementDate, instruments, dayCounter)));

// ++++++++++++++++++  build of the underlying process : with a Black-Scholes model 

	boost::shared_ptr<GeneralizedBlackScholesProcess> bsProcess (new BlackScholesProcess(underlying_priceH,riskFreeTS,flatVolTS));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++ Description of the option +++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	std::string		Option_name="IBM Option";
	Date			maturity(26, Jan,2013);		// INPUT
	Real			strike = 190;				// INPUT
	Option::Type	type (Option::Call);			// INPUT

	boost::shared_ptr<Exercise> europeanExercise(new EuropeanExercise(maturity));

	
	std::cout << "**********************************" << std::endl;
	std::cout << "Description of the option:		" << Option_name		<< std::endl;
	std::cout << "Date of maturity:     			" << maturity			<< std::endl;
	std::cout << "Type of the option:   			" << type				<< std::endl;
	std::cout << "Strike of the option:				" << strike				<< std::endl;



// 
// ++++++++++++++++++ Description of the final payoff 
	boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(type, strike));

// just too test
	VanillaOption europeanOption(payoff, europeanExercise);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++ Description of the pricing  +++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// For the choice of the finite difference model with the CrankNicolson scheme
	//		this model need to precise the time and space step
	//		More they are greater, more the calul will be precise.
	Size GridPoints = 101; // INPUT
	Size TimeSteps	= 100; // INPUT
    
    boost::shared_ptr<PricingEngine> fdEuropeanEngine(
        new FDEuropeanEngine<CrankNicolson>(bsProcess, TimeSteps, GridPoints, true));

	boost::shared_ptr<PricingEngine> europeanEngine(
		new AnalyticEuropeanEngine(bsProcess));


	// ++++++++++++++++++++ Valorisation ++++++++++++++++++++++++++++++++++++++++
		
	// Link the pricing Engine to the option
	europeanOption.setPricingEngine(europeanEngine);
	std::cout << "NPV of the European Option without dividend:          : "	<< europeanOption.NPV() <<  std::endl;
    
    // using FD method on the same option
    europeanOption.setPricingEngine(fdEuropeanEngine); 
    std::cout << "NPV of the European Option without dividend(FD Method): " << europeanOption.NPV() << std::endl; 
    
    //just a single test
	std::cout << "ZeroRate with a maturity at "<<maturity << ": " << riskFreeTS.currentLink()->zeroRate(
																				maturity,dayCounter,Simple) << std::endl;
    std::cout << "Test Finished\n";
	} catch(std::exception& e){
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
};


int main(){

	char pause;
	int dO = fdtest();
	return dO;
}
