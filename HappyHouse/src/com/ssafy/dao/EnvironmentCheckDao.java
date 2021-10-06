package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.EnvironmentCheckDto;

public interface EnvironmentCheckDao {
	List<EnvironmentCheckDto> searchAll();

	List<EnvironmentCheckDto> searchByDongcodeIssue(String dongCode, String issue);
}
