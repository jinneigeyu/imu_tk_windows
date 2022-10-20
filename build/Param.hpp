#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

struct AlgParam
{
	std::string inputImuFile;

	std::array<double, 3>  baise = std::array<double, 3>{0, 0, 0};
	std::array<double, 3>  scale = std::array<double, 3>{1, 1, 1};

	double startTimeSecondsOffset = 2;
	double initTimeDurationSeconds = 12.0;
	double gravityMagnitude = 9.81744;

	std::string accResultFilePath = "acc_cali.txt";
	std::string gyroResultFilePath = "gyro_cali.txt";

	double intervalVarianceStart = 2;
	double intervalVarianceEnd = 2;
	bool showplot = false;
	
	// use json to print
	//friend ostream& operator<<(ostream& stream, const AlgParam& p)
	//{
	//
	//}
};

void to_json(nlohmann::json& j, const AlgParam& p)
{
	j = nlohmann::json{
		{"inputImuFile"					, p.inputImuFile					},
		{"baise"						, p.baise							},
		{"scale"						, p.scale							},
		{"initTimeDurationSeconds"		, p.initTimeDurationSeconds			},
		{"gravityMagnitude"				, p.gravityMagnitude				},
		{"accResultFilePath"			, p.accResultFilePath				},
		{"gyroResultFilePath"			, p.gyroResultFilePath				},
		{"intervalVarianceStart"		, p.intervalVarianceStart			},
		{"intervalVarianceEnd"			, p.intervalVarianceEnd				},
	};
}

void from_json(const nlohmann::json& j, AlgParam& p)
{

	j.at("inputImuFile").get_to(p.inputImuFile);
	j.at("baise").get_to(p.baise);
	j.at("scale").get_to(p.scale);
	j.at("initTimeDurationSeconds").get_to(p.initTimeDurationSeconds);
	j.at("gravityMagnitude").get_to(p.gravityMagnitude);
	j.at("accResultFilePath").get_to(p.accResultFilePath);
	j.at("gyroResultFilePath").get_to(p.gyroResultFilePath);
	j.at("intervalVarianceStart").get_to(p.intervalVarianceStart);
	j.at("intervalVarianceEnd").get_to(p.intervalVarianceEnd);
}