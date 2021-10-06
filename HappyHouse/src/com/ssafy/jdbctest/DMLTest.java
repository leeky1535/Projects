package com.ssafy.jdbctest;

import java.util.List;

import com.ssafy.dao.SanggwonDao;
import com.ssafy.daoImpl.SanggwonDaoImpl;
import com.ssafy.dto.SanggwonDto;

/*
 * JDBC 작업 순서
 * 1. Driver Loading
 * 2. DB 연결 (Connection 생성)
 * 3. SQL 실행 준비
 *   3-1. SQL 작성.
 *   3-2. Statement 생성 (Statement, PreparedStatement)
 * 4. SQL 실행
 *   4-1. I, U, D
 *      int x = stmt.execteUpdate(sql);
 *   	int x = pstmt.executeUpdate();
 *   4-2. S
 *      ResultSet rs = pstmt.executeQuery();
 *      rs.next() [단독, if, while]
 *      값얻기 : rs.getString()
 *            rs.getInt() 등등등....
 * 5. DB 연결 종료 : 연결 역순으로 종료, finally, AutoCloseable, try-with-resource (JDK7이상)
 * 	if(rs != null)
 *    		rs.close()
 *  	if(pstmt != null)
 *  		pstmt.close();
 *  	if(conn != null)
 *  		conn.close();
 */

public class DMLTest {

	public DMLTest() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			System.out.println("드라이버 로딩 성공!!");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		SanggwonDao sanggwonDao = SanggwonDaoImpl.getSanggwonDao();

		List<SanggwonDto> l = sanggwonDao.searchByDongcodeCategoryName("1135010600", "R09A01", "어린이");// 관심사 검색
		for (int i = 0; i < l.size(); i++) {
			System.out.println(l.get(i));
		}
		System.out.println(l.size());
	}
}
