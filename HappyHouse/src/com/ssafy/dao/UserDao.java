package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.UserDto;

public interface UserDao {
	void register(UserDto userDto) throws Exception;

	UserDto searchById(String userId) throws Exception;

	List<UserDto> searchAll() throws Exception;

	void updateUserInfo(UserDto userDto) throws Exception;

	void deleteUser(String userId) throws Exception;

	int usernameCheck(String id) throws Exception;

	UserDto login(String id, String pass) throws Exception;
}
