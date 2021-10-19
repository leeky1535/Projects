package service;

import com.ssafy.dao.UserDao;
import com.ssafy.daoImpl.UserDaoImpl;
import com.ssafy.dto.UserDto;

public class UserServiceImpl implements UserService {

	private static UserService userService = new UserServiceImpl();
	private UserDao userDao;

	private UserServiceImpl() {
		userDao = UserDaoImpl.getUserDao();
	}

	public static UserService getUserService() {
		return userService;
	}

	@Override
	public int usernameCheck(String username) throws Exception {
		return userDao.usernameCheck(username); // 0 or 1
	}

	@Override
	public void register(UserDto userDto) throws Exception {
//		validation check
		userDao.register(userDto);
	}

	@Override
	public UserDto login(String id, String pass) throws Exception {
		return userDao.login(id, pass);
	}

	@Override
	public UserDto getUser(String username) throws Exception {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void updateUser(UserDto userDto) throws Exception {
		userDao.updateUserInfo(userDto);

	}

	@Override
	public void deleteUser(String username) throws Exception {
		// TODO Auto-generated method stub

	}

}
