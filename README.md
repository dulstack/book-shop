<html>
 <body>
  <h1>Book shop</h1><hr/>
  <p>The simple project for simulating the book selling/purchasing with REST API</p>
  <hr/>
  <h2>Bank API</h2>
  <p>Bank API has the following function scripts:</p>
  <ul>
   <li>(POST)/api/create_account -- variables: email, pwd</li>
   <li>(POST)/api/log_in         -- variables: email, pwd</li>
   <li>(POST)/api/auth           -- cookies: id, hash</li>
   <li>(POST)/api/deposit        -- cookies: id, hash; variables: ammount</li>
   <li>(POST)/api/transfer       -- cookies: id, hash, variables: id, ammount</li>
   <li>(GET)/api/get_balance     -- variables: id</li>
  </ul>
 </body>
</html>
